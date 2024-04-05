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
        typedef std::string HeaderName;

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
         * This method constructs and returns the raw string
         * message based on the headers parts that have been
         * collected by the object
         * 
         * @return 
         *      The raw string Message based on the 
         *      headers parts is returned
        */
        std::string GenerateRawHeaders() const;

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
}

#endif /* MESSAGE_HEADERS_HPP */
