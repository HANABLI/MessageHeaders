/**
 * @file InternetMessage.cpp
 * 
 * This module containe the implementation of the InternetMessage::InternetMessage class.
 * 
 * © 2024 by Hatem Nabli
*/

#include<InternetMessage/InternetMessage.hpp>

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

namespace InternetMessage {

    /**
     * This contains the private properties of Uri instance.
    */
    struct InternetMessage::Impl
    {
        /**
         * These are the headers of the internet message.
        */
        Headers headers;

        /**
         * This is the body of the message.
        */
        std::string body;
    };

    InternetMessage::~InternetMessage() = default;

    InternetMessage::InternetMessage():impl_(new Impl) {

    }

    bool InternetMessage::ParseFromString(const std::string& rawMessage) {
        size_t offset = 0;
        while(offset < rawMessage.length()) {
            auto lineTerminator = rawMessage.find("\r\n", offset);
            if (lineTerminator == std::string::npos) {
                break;
            }
            if (lineTerminator - offset > 998) {
                return false;
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
        impl_->body = rawMessage.substr(offset);
        bool lastCharacterCR = false;
        bool lastCharacterLF = false;
        for (auto c: impl_->body) {
            if (c == '\r') {
                lastCharacterCR = true;
            } else if (
                (c == '\n') == !lastCharacterCR 
            ) {
                        return false;
            } else {
                lastCharacterCR = false;
            }
        }
        if (lastCharacterCR) {
            return false;
        }
        return true;
    }

    auto InternetMessage::GetHeaders() const -> Headers {
        return impl_->headers;
    }

    bool InternetMessage::HasHeader(const HeaderName& name) const {
        for (const auto& header: impl_->headers) {
            if (header.name == name) {
                return true;
            }
        }
        return false;
    }

    auto InternetMessage::GetHeaderValue(const HeaderName& headerName) const -> HeaderValue {
        for (const auto& header: impl_->headers) {
            if (header.name == headerName) {
                return header.value;
            }
        }
        return "";
    }

    std::string InternetMessage::GetBody() const {
        return impl_->body;
    }

    std::string InternetMessage::GenerateRawMessage() const {
        std::ostringstream rawMessage;
        for (const auto& header: impl_->headers) {
            rawMessage << header.name << ": " << header.value << "\r\n";
        }
        rawMessage << "\r\n";
        rawMessage << impl_->body;

        return rawMessage.str();
    }

}