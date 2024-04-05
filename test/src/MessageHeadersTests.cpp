/**
 * @file MessageHeadersTests.cpp
 * 
 * This module contains unit Tests of the MessageHeaders::MessageHeaders class 
 * 
 * © 2024 by Hatem Nabli
*/

#include <gtest/gtest.h>
#include <MessageHeaders/MessageHeaders.hpp>
#include <string>
#include <vector>

TEST(MessageHeadersTests, PlaceHolder_Test) {
    MessageHeaders::MessageHeaders headers;
    ASSERT_TRUE(true);
}

TEST(MessageHeadersTests, HttpClientRequestMessage) {
    MessageHeaders::MessageHeaders headers;
    const std::string rawMessage = "User-Agent: curl/7.16.3 libcurl/7.163 OpenSSL/0.9.7l zlib/1.2.3\r\n"
        "Host: www.example.com\r\n"
        "Accept-Language: en, mi\r\n"
        "\r\n";
    ASSERT_TRUE(headers.ParseRawMessage(rawMessage));
    const auto headersCollection = headers.GetAll();
    struct ExpectedHeader {
        std::string name;
        std::string value;
    };
    const std::vector< ExpectedHeader > expectedHeaders {
        {"User-Agent", "curl/7.16.3 libcurl/7.163 OpenSSL/0.9.7l zlib/1.2.3"},
        {"Host", "www.example.com"},
        {"Accept-Language", "en, mi"},
    };
    
    ASSERT_EQ(expectedHeaders.size(), headersCollection.size());
    for(size_t i = 0; i < expectedHeaders.size(); ++i) {
        ASSERT_EQ(expectedHeaders[i].name, headersCollection[i].name);
        ASSERT_EQ(expectedHeaders[i].value, headersCollection[i].value);
    }
    ASSERT_TRUE(headers.HasHeader("Host"));
    ASSERT_FALSE(headers.HasHeader("Toto"));
}

TEST(MessageHeadersTests, HttpServerResponseMessage) {
    MessageHeaders::MessageHeaders headers;
    const std::string rawHeaders = "Date: Mon, 27 Jul 2009 12:28:53 GMT\r\n"
            "Server: Apache\r\n"
            "Last-Modified: Wed, 22 Jul 2009 18:12:53 GMT\r\n"
            "ETag: \"34aa387-d-1568eb00\"\r\n"
            "Accept-Ranges: bytes\r\n"
            "Content-Length: 51\r\n"
            "Vary: Accept-Encoding\r\n"
            "Content-Type: text/plain\r\n"
            "\r\n";
    const std::string rawMessage = (
        rawHeaders + "Hello World! My payload includes a trailing CRLF.\r\n"
    );
    size_t bodyOffset;
    ASSERT_TRUE(
        headers.ParseRawMessage(rawMessage, bodyOffset)
    );
    ASSERT_EQ(rawHeaders.length(), bodyOffset);
    const auto headersCollection = headers.GetAll();
    struct ExpectedHeader {
        std::string name;
        std::string value;
    };
    const std::vector< ExpectedHeader > expectedHeaders {
        {"Date", "Mon, 27 Jul 2009 12:28:53 GMT"},
        {"Server", "Apache"},
        {"Last-Modified", "Wed, 22 Jul 2009 18:12:53 GMT"},
        {"ETag", "\"34aa387-d-1568eb00\""},
        {"Accept-Ranges", "bytes"},
        {"Content-Length", "51"},
        {"Vary", "Accept-Encoding"},
        {"Content-Type", "text/plain"}
    };
    ASSERT_EQ(expectedHeaders.size(), headersCollection.size());
    for(size_t i = 0; i < expectedHeaders.size(); ++i) {
        ASSERT_EQ(expectedHeaders[i].name, headersCollection[i].name);
        ASSERT_EQ(expectedHeaders[i].value, headersCollection[i].value);
    }
    ASSERT_TRUE(headers.HasHeader("Date"));
    ASSERT_FALSE(headers.HasHeader("Toto"));
    ASSERT_EQ(rawHeaders, headers.GenerateRawHeaders());
}

TEST(MessageHeadersTests, HeadersLineAlmostTooLong) {
    MessageHeaders::MessageHeaders headers;
    const std::string testHeaderName("X-Poggers");
    const std::string testHeaderNameWithDelimiters = testHeaderName + ": ";
    const std::string longestPossiblePoggers(998 - testHeaderNameWithDelimiters.length(), 'X');
    const std::string rawMessage = (
        "User-Agent: curl/7.16.3 libcurl/7.163 OpenSSL/0.9.7l zlib/1.2.3\r\n"
        "Host: www.example.com\r\n"
        + testHeaderNameWithDelimiters + longestPossiblePoggers + "\r\n"
        "Accept-Language: en, mi\r\n"
        "\r\n"
    );
    ASSERT_TRUE(headers.ParseRawMessage(rawMessage));
}

TEST(MessageHeadersTests, HeadersLineTooLong) {
    MessageHeaders::MessageHeaders headers;
    headers.SetLineLimit(1000);
    const std::string testHeaderName("X-Poggers");
    const std::string testHeaderNameWithDelimiters = testHeaderName + ": ";
    const std::string longestPossiblePoggers(999 - testHeaderNameWithDelimiters.length(), 'X');
    const std::string rawMessage = (
        "User-Agent: curl/7.16.3 libcurl/7.163 OpenSSL/0.9.7l zlib/1.2.3\r\n"
        "Host: www.example.com\r\n"
        + testHeaderNameWithDelimiters + longestPossiblePoggers + "\r\n"
        "Accept-Language: en, mi\r\n"
        "\r\n"
    );
    ASSERT_FALSE(headers.ParseRawMessage(rawMessage));
}

TEST(MessageHeadersTests, HeaderLineOver1000CharactersAllawedByDeafault) {
    MessageHeaders::MessageHeaders headers;
    const std::string testHeaderName("X-Poggers");
    const std::string testHeaderNameWithDelimiters = testHeaderName + ": ";
    const std::string headersValuesLongerThan1000Characters(999 - testHeaderNameWithDelimiters.length(), 'X');
    const std::string rawMessage = (
        "User-Agent: curl/7.16.3 libcurl/7.163 OpenSSL/0.9.7l zlib/1.2.3\r\n"
        "Host: www.example.com\r\n"
        + testHeaderNameWithDelimiters + headersValuesLongerThan1000Characters + "\r\n"
        "Accept-Language: en, mi\r\n"
        "\r\n"
    );
    ASSERT_TRUE(headers.ParseRawMessage(rawMessage));
    ASSERT_EQ(headersValuesLongerThan1000Characters, headers.GetHeaderValue(testHeaderName));
}
TEST(MessageHeadersTests, GetValueOfPresentHeader) {
    MessageHeaders::MessageHeaders headers;
    const std::string rawMessage = "User-Agent: curl/7.16.3 libcurl/7.163 OpenSSL/0.9.7l zlib/1.2.3\r\n"
        "Host: www.example.com\r\n"
        "Accept-Language: en, mi\r\n"
        "\r\n";
    ASSERT_TRUE(headers.ParseRawMessage(rawMessage));
    ASSERT_EQ("www.example.com", headers.GetHeaderValue("Host"));
}

TEST(MessageHeadersTests, GetValueOfMissingHeader) {
    MessageHeaders::MessageHeaders headers;
    const std::string rawMessage = "User-Agent: curl/7.16.3 libcurl/7.163 OpenSSL/0.9.7l zlib/1.2.3\r\n"
        "Host: www.example.com\r\n"
        "Accept-Language: en, mi\r\n"
        "\r\n";
    ASSERT_TRUE(headers.ParseRawMessage(rawMessage));
    ASSERT_EQ("", headers.GetHeaderValue("toto"));
}

TEST(MessageHeadersTests, SetHeaderAdd) {
    MessageHeaders::MessageHeaders headers;
    const std::string rawMessage = "User-Agent: curl/7.16.3 libcurl/7.163 OpenSSL/0.9.7l zlib/1.2.3\r\n"
        "Host: www.example.com\r\n"
        "Accept-Language: en, mi\r\n"
        "\r\n";
    ASSERT_TRUE(headers.ParseRawMessage(rawMessage));
    headers.SetHeader("TOTO", "titi");
    ASSERT_EQ("titi", headers.GetHeaderValue("TOTO"));
}

TEST(MessageHeadersTests, SetHeaderModify) {
    MessageHeaders::MessageHeaders headers;
    const std::string rawMessage = "User-Agent: curl/7.16.3 libcurl/7.163 OpenSSL/0.9.7l zlib/1.2.3\r\n"
        "Host: www.example.com\r\n"
        "Accept-Language: en, mi\r\n"
        "\r\n";
    ASSERT_TRUE(headers.ParseRawMessage(rawMessage));
    headers.SetHeader("Host", "www.newExample.com");
    ASSERT_EQ("www.newExample.com", headers.GetHeaderValue("Host"));
}

TEST(MessageHeadersTests, HeaderWithNonAsciiCharacterIntheName) {
    MessageHeaders::MessageHeaders headers;
    const std::string rawMessage = "User-Agent: curl/7.16.3 libcurl/7.163 OpenSSL/0.9.7l zlib/1.2.3\r\n"
        "Host: www.example.com\r\n"
        "Feels Bad: It's really Bad"
        "Accept-Language: en, mi\r\n"
        "\r\n";
    ASSERT_FALSE(headers.ParseRawMessage(rawMessage));
}

TEST(MessageHeadersTests, UnfoldingHeaderValue) {
    MessageHeaders::MessageHeaders headers;
    const std::string rawMessage = "User-Agent: curl/7.16.3 libcurl/7.163 OpenSSL/0.9.7l zlib/1.2.3\r\n"
        "Host: www.example.com\r\n"
        "Accept-Language: en, mi\r\n"
        "Subject: This\r\n"
        " is a test\r\n"
        "\r\n";
    ASSERT_TRUE(headers.ParseRawMessage(rawMessage));
    ASSERT_EQ("This is a test", headers.GetHeaderValue("Subject"));
}

TEST(MessageHeadersTests, FoldLineThatWouldExceedLimit) {
    const std::string headerName = "X";
    struct TestVector {
        std::string headerValue;
        std::vector< std::string > expectedLines;
    };
    std::vector< TestVector > testVectors {
        {"Hello, World!",        {"X: Hello,", " World!",   ""              }},
        {"This is even longer!", {"X: This is", " even", " longer!",   ""}},
        {"aaadadazdadcvbfdfvdf", {                                 ""}}

    };
    size_t index = 0;
    for (const auto& test: testVectors) {
        MessageHeaders::MessageHeaders headers;
        headers.SetLineLimit(12);
        headers.SetHeader(headerName, test.headerValue);
        const auto rawHeaders = headers.GenerateRawHeaders();
        std::vector< std::string > actualLines;
        size_t offset = 0;
        while(offset < rawHeaders.length()) {
            auto lineTerminator = rawHeaders.find("\r\n", offset);
            if (lineTerminator == std::string::npos) {
                break;
            }
            const auto line = rawHeaders.substr(
                offset,
                lineTerminator - offset
            );
            actualLines.push_back(line);
            offset = lineTerminator + 2;
        }
        ASSERT_EQ(test.expectedLines, actualLines) << index;
        ++index;
    }
}
