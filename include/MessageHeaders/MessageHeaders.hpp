#ifndef MESSAGE_HEADERS_HPP
#define MESSAGE_HEADERS_HPP
/**
 * @file MessageHeaders.hpp
 * 
 * This module contains the declaration of the MessageHeaders::MessageHeaders class.
 * 
 * © 2024 by Hatem Nabli
*/

#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include <ostream>
#include <functional>
#include <ctype.h>





namespace MessageHeaders {
    


    /**
     * This class represents the headers of a message format on the internet
     * which is a common theme amongst several internet standards:
     * - e-mail : as defined in RFC 5322 (https://tools/ieft.org/html/rfc5322).
     * - HTTP (web): as defined in RFC 7230 (https://tools/ieft.org/html/rfc7230).
     * - SIP (VoIP): as defined in RFC 3261 (https://tools/ieft.org/html/rfc3261)
    */

    class MessageHeaders {

    public:
        /**
         * This is how we handle the name of an Message header.
        */
        class HeaderName{
            // Public Methods
        public:
            ~HeaderName() noexcept = default;
            HeaderName(const HeaderName& s) = default;
            HeaderName(HeaderName&& s) noexcept = default;
            HeaderName& operator=(const HeaderName&) = default;
            HeaderName& operator=(HeaderName&&) = default;

        public:
            /**
             * This is the default constractor
             * 
            */
            HeaderName() = default;

            /**
             * This constructs the header name based on a normal C++ string.
             * 
             * @param[in] s
             *      This si the name to set for the header name.
            */
            HeaderName(const std::string& s);
            
            /**
             * This constructs the header name based on a normal C string.
             * 
             * @param[in] s
             *      This si the name to set for the header name.
            */
            HeaderName(const char* s) : name_(s) {}

            /**
             * This assignment operator assigns the header name
             * based on a normal string.
             * 
             * @param[in] s
             *      This is the name to set for the header name.
             * 
             * @return
             *      A reference to the object is returned.
            */
             HeaderName& operator=(const std::string& s);

            /**
             * This is the equality operator for the class.
             * 
             * @param[in] rhs
             *      This is the other header name with which to compare.
             * 
             * @return
             *      returns an indication of whether or not headers names
             *      are equivalent (case-insensitive).
            */
            bool operator==(const HeaderName& rhs) const ;

            /**
             * This is used in range-for constructs, to get the beginning
             * iterator of the sequence. It's merely going to forward to 
             * underlying C++ string holding the name text.
             * 
             * @return 
             *      returns the beginning iterator of the sequence.
            */
            auto begin() const;

            /**
             * This is used in range-for constructs, to get the ending
             * iterator of the sequence. It's merely going to forward to 
             * underlying C++ string holding the name text.
             * 
             * @return 
             *      returns the ending iterator of the sequence.
            */
            auto end() const;

            /**
             * This is the type cast operator to c++ string
             * 
             * @return
             *      returns a C++ string equivalent to the header name.
            */
            operator const std::string&() const;
            // Private Properties
        private:
            /**
             * This is the content of the headerName.
            */
           std::string name_;

        };

        typedef std::string HeaderValue;
        /**
         * This represent a singel header of the Message.
        */
        struct Header {
            /**
             * This is the part of the header that come before the colon.
             * It identifies the purpose of the header.
            */
           
            HeaderName name;
            /**
             * This is the part of the header that comes after the colon.
             * It provides the value, setting, or context whose meaning
             * depends on the header name.
            */
            HeaderValue value;

            Header(
                const HeaderName& newName,
                const HeaderValue& newValue
            );
        };

        /**
         * This represents the collection of all headers
         * of the Message.
        */
        typedef std::vector< Header > Headers;
        // Lifecycle management
    public:
        ~MessageHeaders();
        MessageHeaders(const MessageHeaders&) = delete; // Copy Constructor that creates a new object by making a copy of an existing object. 
        //It ensures that a deep copy is performed if the object contains dynamically allocated resources 
        MessageHeaders(MessageHeaders&&); // Move Constructor that transfers resources from an expiring object to a newly constructed object.
        MessageHeaders& operator=(const MessageHeaders&) = delete; //Copy Assignment Operation That assigns the values of one object to another object using the assignment operator (=)
        MessageHeaders& operator=(MessageHeaders&&); //Move Assignment Operator: Amove assignment operator efficiently transfers resources from one object to another.
        
        //public methods
    public:
        /**
         * This is the default constructor
        */
       MessageHeaders();

        /**
         * This is the equality comparison operator for the class
         * 
         * @param[in] otherInternetMessage
         *      This is the other MessageHeaders to which to compare 
         *      this MessageHeaders.
         * @return
         *      An indication of whether or not this MessageHeaders is 
         *      equal to the other one
        */
       bool operator==(const MessageHeaders& otherInternetMessage) const;
        /**
         * This is the inequality comparison operator for the class
         * 
         * @param[in] otherInternetMessage
         *      This is the other MessageHeaders to which to compare 
         *      this MessageHeaders.
         * @return
         *      An indication of whether or not this MessageHeaders is 
         *      equal to the other one
        */
       bool operator!=(const MessageHeaders& otherInternetMessage) const;

       /**
        * This method build the Message by determining the headers 
        * parts from the elements parsed from the given string
        * rendering of an Message.
        * 
        * @param[in] rawMessageString
        *       This is the string rendering of the Message to parce.
        * @param[out] bodyOffset
        *       This is where to store the offset into the given
        *       raw message where the headers ended and the body begins.
        * 
        * @return
        *       whether or not the Message was parsed successfully 
        *       is returned.
       */
       bool ParseRawMessage(const std::string& rawMessageString, size_t& bodyOffset);

             /**
        * This method build the Message by determining the headers 
        * parts from the elements parsed from the given string
        * rendering of an Message.
        * 
        * @param[in] rawMessageString
        *       This is the string rendering of the Message to parce.
        * 
        * @return
        *       whether or not the Message was parsed successfully 
        *       is returned.
       */
       bool ParseRawMessage(const std::string& rawMessageString);

        /**
         * This method returns the collection of Headers elements of the Message.
         * 
         * @return
         *      The collection of Headers elements of the Message.
         * @retval ""
         *      An empty collection is returned if there is no "Header" element 
         *      in the Message.
        */
       Headers GetAll() const;

        /**
         * This method checks if there is a header with the given name
         * in the message.
         * 
         * @param[in] headerName
         *      This is the header name to check if it's present in the message.
         * 
         * @return 
         *      return an indication of whether or not an Message have
         *      a Header.
        */
        bool HasHeader(const HeaderName& headerName) const;
        
        /**
         * This method returns the value for the header with the given
         * name in the message.
         * 
         * @param[in] headerName
         *      This is the name of the header whose value should be returned
         * 
         * @return
         *      returns the value of the given header name.
         * 
         * @note   
         *      If there is no header with the given name in the message
         *      then just an empty string is returned.
        */
        HeaderValue GetHeaderValue(const HeaderName& headerName) const;

        /**
         * This method returns a collection of values for the header with the given
         * name in the message.
         * 
         * @param[in] headerName
         *      This is the name of the header whose values should be returned
         * 
         * @return
         *      returns the values of the given header name.
         * 
         * @retval {}   
         *      If there is no header with the given name in the message
         *      then just an empty collection is returned.
        */
        std::vector< HeaderValue > GetHeaderMultiValues(const HeaderName& headerName) const;
        
        /**
         * This method add or modifie the header with the given name,
         * to have the given one.
         * 
         * @param[in] name
         *      This is the name of the header to add or replace.
         * 
         * @param[in] value
         *      This is the value of the header to add or replace.
        */
        void SetHeader( const HeaderName& name, const HeaderValue& value);
        
        /**
         * This method add or modifie the header with the given name,
         * to have the given values.
         * 
         * @param[in] name
         *      This is the name of the header to add or replace.
         * 
         * @param[in] values
         *      This is the collection of values of the header to add or replace.
         * 
         * @param[in] oneLine
         *      This specifies whether or not to combine values into on header line, separated by colons.
        */
        void SetHeader( const HeaderName& name, const std::vector< HeaderValue >& values, bool oneLine);

        /**
         * This method add  the header with the given name,
         * to have the given value.
         * 
         * @param[in] name
         *      This is the name of the header to add.
         * 
         * @param[in] values
         *      This is the value of the header to add.
         * 
        */
       void AddHeader( const HeaderName& name, const HeaderValue& value);
        
        /**
         * This method adds  the header with the given name,
         * to have the given values.
         * 
         * @param[in] name
         *      This is the name of the header to add.
         * 
         * @param[in] values
         *      This is the values of the header to add.
         * 
        */
       void AddHeader( 
            const HeaderName& name, 
            const std::vector< HeaderValue >& values, 
            bool oneLine
        );

        /**
         * This method constructs and returns the raw string
         * message based on the headers parts that have been
         * collected by the object
         * 
         * @return 
         *      The raw string Message based on the 
         *      headers parts is returned
        */
        std::string GenerateRawHeaders() const;

        /**
         * This method sets a limit for the number of characters
         * in any header line.
         * 
         * @param[in] newLineLengthLimit
         *      This is the maximum number of characters, including
         *      the 2-characters CRLF line terminator, that should
         *      be allowed for a single header line.
        */
        void SetLineLimit(size_t lineLengthLimit);
       //private properties
    private:
        /**
         * This is the type of structure that contains the private
         * properties of the instance. It is defined in the implementation
         * and declared here to ensure that iwt is scoped inside the class.
        */
       struct Impl;

       /**
        * This contains the private properties of the instance.
        */       
       std::unique_ptr<struct Impl> impl_;
   };

    /**
     * This is a support function for the MessageHeaders::HeaderName class.
     * It's used when header names are shifted out to output streams.
     * 
     * @param[in] stream
     *      This is the stream to which we are shifting out the header name.
     * 
     * @param[in] name
     *      This is the header name to shift out.
     * 
     * @return
     *      The output stream is returned in order to support operation chaining.
    */
   std::ostream& operator<<(std::ostream& stream, const MessageHeaders::HeaderName& name);
   
    /**
     * This is a support function for the MessageHeaders::HeaderName class.
     * It's used when comparing a string to a header name, and the string
     * is on the left-hand side of the operation.
     * 
     * @param[in] lhs
     *      This is the stream to compare with the header name.
     * 
     * @param[in] rhs
     *      This is the header name compare with the stream.
     * 
     * @return
     *      returns an indication of whether or not the string and header name
     *      are equals.
    */
   bool operator==(
    const std::string& lhs,
    const MessageHeaders::HeaderName& rhs
    );

    /**
     * This is a support function for Google Test used to print out
     * values of the MessageHeaders::HeaderName class.
     * 
     * @param[in] name
     *      This is the header name to print.
     * 
     * @param[in] os
     *      This points to the stream to whish to print the name
    */
   void PrintTo(
        const MessageHeaders::HeaderName& name,
        std::ostream* os
   );
}

#endif /* MESSAGE_HEADERS_HPP */
