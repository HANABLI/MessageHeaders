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
}

namespace MessageHeaders {

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
            auto lineTerminator = rawMessage.find("\r\n", offset);
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
                auto nextLineTerminator = rawMessage.find("\r\n", nextLineStart);
                if (nextLineTerminator == std::string::npos) {
                    break;
                }
                const auto nextLineLength = nextLineTerminator - nextLineStart;
                if (
                    (nextLineLength > 2)
                    && (WSP.find(rawMessage[nextLineStart]) != std::string::npos)
                ) {
                    value += rawMessage.substr(nextLineStart, nextLineLength);
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
            rawMessage << header.name << ": " << header.value << "\r\n";
        }
        rawMessage << "\r\n";
        return rawMessage.str();
    }

    void MessageHeaders::SetLineLimit(size_t lineLengthLimit) {
        impl_->lineLengthLimit = lineLengthLimit;
    }
}