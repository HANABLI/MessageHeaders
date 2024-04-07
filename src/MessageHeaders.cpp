/**
 * @file MessageHeaders.cpp
 * 
 * This module containe the implementation of the MessageHeaders::MessageHeaders class.
 * 
 * © 2024 by Hatem Nabli
*/

#include<MessageHeaders/MessageHeaders.hpp>

namespace {

    /**
     * These are the characters that are considered whitespace and
     * should be stripped off by the StripMarginWhitespace() function.
     * 
     * The name of "WSP" was chosen to match the symbol name from
     * RFC 2822 (https://tools/ieft.org/html/rfc2822) which refers
     * to this specific character set.
    */
    const std::string WSP = " \t";

    /**
     * This is the required line terminator for internet message header lines.
    */
    const std::string CRLF = "\r\n";


    /**
     * 
    */
   std::string StripMarginWhitespace(const std::string& s) {
        const auto marginLeft = s.find_first_not_of(WSP);
        const auto marginRight = s.find_last_not_of(WSP);
        if (marginLeft == std::string::npos) {
            return "";
        } else {
            return s.substr(marginLeft, marginRight - marginLeft + 1);
        }
   }

    /**
     * This is the type of function that is used as the strategy to
     * determine where to break a long line into two smaller lines.
     * 
     * @param[in] s
     *      This is the string which we are considering breaking.
     * 
     * @param[in] startOffset
     *      This is the offset into the given string of the beginning of the part
     *      that is under consideration for breaking. 
     * 
     * @param[out] breakOffset  
     *      When the string is broken into two parts, this is the offset
     *      in the original string marking the end of the first part.
     * 
     * @param[out] nextOffset  
     *      When the string is broken into two parts, this is the offset
     *      in the original string marking the beginning of the second part.
     * @return 
     *      returns an indication of whether or not the string can be broken.
    */
    typedef std::function <bool(
        const std::string& s, 
        size_t startOffset,
        size_t& breakOffset,
        size_t& nextOffset
        )> StringBreakingStrategy;

    /**
     * This method breaks up the given input line into multiple output lines,
     * as needed, tp ensure that no output line is longer than the given
     * line limit, including line terminators.
     * 
     * @param[in] line
     *      This is the line to fold if necessary.
     * @param[in] terminator
     *      This is the character sequence that separates lines.
     * @param[in] continuator
     *      This is the string to preprend to each output 
     *      line after the first.
     * @param[in] limit
     *      This is the maximum number of characters allowed par line,
     *      including the line terminator.
     * @param[in] lineFoldingStrategy
     *      This is the function to call that will determine
     *      where (if anywhere) to break up the input line.
     * @return
     *      The output lines are returned.
     * 
     * @retval  
     *      This is returned if the line could not be folded into
     *      multiple lines.
     */
   std::vector< std::string > SplitLine( 
        const std::string& inputLline, 
        const std::string& terminator,
        const std::string& continuator, 
        StringBreakingStrategy lineFoldingStrategy
    ) {
        std::vector< std::string > output;
        size_t breakOffset = 0;
        size_t currentLineStart = 0;
        while(currentLineStart < inputLline.length()) {
            size_t nextLineStart;
            if (!lineFoldingStrategy(
                inputLline,
                currentLineStart,
                breakOffset,
                nextLineStart
            )) {
                return {};
            }
            std::string part;
            if (currentLineStart != 0) {
                part = continuator;
            }
            part += inputLline.substr(currentLineStart, breakOffset - currentLineStart);
            if (
                (part.length() < terminator.length()) 
                || (part.substr(part.length() - terminator.length()) != terminator)
            ) {
                part += terminator;
            }
            output.push_back(part);
            currentLineStart = breakOffset + 1;
        }
        return output;
   }
}

namespace MessageHeaders {

   std::ostream& operator<<(
        std::ostream& stream, 
        const MessageHeaders::HeaderName& name
    ) {
        return stream << (std::string)name;
    }
   bool operator==(
        const std::string& lhs,
        const MessageHeaders::HeaderName& rhs
    ) {
        return rhs == lhs;
    }

    void PrintTo(
        const MessageHeaders::HeaderName& name,
        std::ostream* os
   ) {
        *os << name;
   }

    MessageHeaders::HeaderName::HeaderName(const std::string& s) : name_(s) {}

    bool MessageHeaders::HeaderName::operator==(const HeaderName& rhs) const 
    { 
        return name_ == rhs.name_;
    }

    auto MessageHeaders::HeaderName::begin() const 
    { 
        return name_.begin(); 
    }

    auto MessageHeaders::HeaderName::end() const 
    {
        return name_.end(); 
    }

    //cast operator
    MessageHeaders::HeaderName::operator const  std::string&() const 
    { 
        return name_ ;
    }
    /**
     * This contains the private properties of Uri instance.
    */

   MessageHeaders::Header::Header(
        const HeaderName& newName,
        const HeaderValue& newValue
    )
        : name(newName)
        , value(newValue)
    {
    }

    struct MessageHeaders::Impl
    {
        /**
         * These are the headers of the internet message.
        */
        Headers headers;
        /**
         * This is the maximum number of characters, including
         * the 2-characters CRLF line terminator, that should
         * be allowed for a single header line.
        */
        size_t lineLengthLimit = 0;

        /**
         * This function returns a string splitting strategy 
         * function object which can be used once to foald a 
         * header line.
         * 
         * @return
         *      A string splitting strategy function object 
         *      which can be used once to fold a header line
         *      is returned.
        */
        StringBreakingStrategy MakeHeaderLineFoldingStrategy() {
            auto firstPart = std::make_shared< bool >(true);
            return [this, firstPart](
                const std::string& s, 
                size_t startOffset,
                size_t& breakOffset,
                size_t& nextOffset
            ) {
                if (s.length() - startOffset <= lineLengthLimit) {
                    breakOffset = nextOffset = s.length();
                    return true;
                }
                breakOffset = startOffset;
                const auto reservedCharacters = 2 + (*firstPart ? 0 : 1);
                for (size_t i = startOffset; i <= startOffset + lineLengthLimit - reservedCharacters; ++i) {
                    if ((s[i] == ' ') || (s[i] == '\t')) {
                        if (*firstPart) {
                            *firstPart = false;
                        } else {
                            breakOffset = i;
                        }
                    }
                }
                nextOffset = breakOffset + 1;
                return (breakOffset != startOffset);
            };
        }
    };

    MessageHeaders::~MessageHeaders() = default;

    MessageHeaders::MessageHeaders():impl_(new Impl) {

    }

    bool MessageHeaders::ParseRawMessage(
        const std::string& rawMessage,
        size_t& bodyOffset 
    ) {
        size_t offset = 0;
        while(offset < rawMessage.length()) {
            auto lineTerminator = rawMessage.find(CRLF, offset);
            if (lineTerminator == std::string::npos) {
                break;
            }
            if (impl_->lineLengthLimit > 0) {
                if (lineTerminator - offset + 2> impl_->lineLengthLimit - 2) {
                    return false;
                }
            }
            if (lineTerminator == offset) {
                offset += 2;
                break;
            }

            auto nameValueDelimiter = rawMessage.find(':', offset);
            if (nameValueDelimiter == std::string::npos) {
                return false;
            }
            HeaderName name;
            HeaderValue value;
            name = rawMessage.substr(offset, nameValueDelimiter - offset);
            for (auto c: name) {
                if (
                    (c < 33) || (c >126)
                ) {
                    return false;
                }
            }
            value = StripMarginWhitespace(
                rawMessage.substr(nameValueDelimiter + 1, lineTerminator - nameValueDelimiter - 1)
            );
            offset = lineTerminator + 2;
            for(;;) {
                const auto nextLineStart = lineTerminator + 2;
                auto nextLineTerminator = rawMessage.find(CRLF, nextLineStart);
                if (nextLineTerminator == std::string::npos) {
                    break;
                }
                auto nextLineLength = nextLineTerminator - nextLineStart;
                if (
                    (nextLineLength > 2)
                    && (WSP.find(rawMessage[nextLineStart]) != std::string::npos)
                ) {
                    value += ' ';
                    const auto firstNonWhiteSpaceInNextLine = rawMessage.find_first_not_of(WSP, nextLineStart);
                    nextLineLength -= (firstNonWhiteSpaceInNextLine - nextLineStart);
                    value += rawMessage.substr(firstNonWhiteSpaceInNextLine, nextLineLength);
                    offset = nextLineTerminator + 2;
                    lineTerminator = nextLineTerminator;
                } else {
                    break;
                }
            }
            value = StripMarginWhitespace(value);
            impl_->headers.push_back({name, value});
        }
        bodyOffset = offset;
        return true;
    }

    bool MessageHeaders::ParseRawMessage(const std::string& rawMessageString) {
        size_t bodyOffset;
        return ParseRawMessage(rawMessageString, bodyOffset);
    }

    auto MessageHeaders::GetAll() const -> Headers {
        return impl_->headers;
    }

    bool MessageHeaders::HasHeader(const HeaderName& name) const {
        for (const auto& header: impl_->headers) {
            if (header.name == name) {
                return true;
            }
        }
        return false;
    }

    void MessageHeaders::SetHeader( const HeaderName& name, const HeaderValue& value) {
        for (auto& header: impl_->headers) {
            if (header.name == name) {
                header.value = value;
                return;
            }
        }
        impl_->headers.emplace_back(name, value);
    }

    auto MessageHeaders::GetHeaderValue(const HeaderName& headerName) const -> HeaderValue {
        for (const auto& header: impl_->headers) {
            if (header.name == headerName) {
                return header.value;
            }
        }
        return "";
    }


    std::string MessageHeaders::GenerateRawHeaders() const {
        std::ostringstream rawMessage;
        for (const auto& header: impl_->headers) {
            std::ostringstream lineBuffer;
            lineBuffer << header.name << ": " << header.value << CRLF;
            if (impl_->lineLengthLimit > 0) {
                for (const auto& part: SplitLine(lineBuffer.str(), 
                CRLF, " ", impl_->MakeHeaderLineFoldingStrategy())) {
                    rawMessage << part;
                }
            } else {
            rawMessage << lineBuffer.str();
            }
        }
        rawMessage << CRLF;
        return rawMessage.str();
    }

    void MessageHeaders::SetLineLimit(size_t lineLengthLimit) {
        impl_->lineLengthLimit = lineLengthLimit;
    }
}