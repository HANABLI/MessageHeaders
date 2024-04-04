#ifndef INTERNET_MESSAGE_HPP
#define INTERNET_MESSAGE_HPP
/**
 * @file InternetMesagge.hpp
 * 
 * This module contains the declaration of the InternetMessage::InternetMessage class.
 * 
 * © 2024 by Hatem Nabli
*/

#include <memory>
#include <vector>
#include <string>


namespace InternetMessage {
    

    /**
     * This class represents an Internet Message Format (IMF),
     * as defined in RFC 2822 (https://tools/ieft.org/html/rfc2822).
    */

    class InternetMessage {

    public:
        /**
         * This is how we handle the name of an internet message header.
        */
        typedef std::string HeaderName;

        typedef std::string HeaderValue;
        /**
         * This represent a singel header of the internet message.
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
         * of the internet message.
        */
        typedef std::vector< Header > Headers;
        // Lifecycle management
    public:
        ~InternetMessage();
        InternetMessage(const InternetMessage&) = delete; // Copy Constructor that creates a new object by making a copy of an existing object. 
        //It ensures that a deep copy is performed if the object contains dynamically allocated resources 
        InternetMessage(InternetMessage&&); // Move Constructor that transfers resources from an expiring object to a newly constructed object.
        InternetMessage& operator=(const InternetMessage&) = delete; //Copy Assignment Operation That assigns the values of one object to another object using the assignment operator (=)
        InternetMessage& operator=(InternetMessage&&); //Move Assignment Operator: Amove assignment operator efficiently transfers resources from one object to another.
        
        //public methods
    public:
        /**
         * This is the default constructor
        */
       InternetMessage();

        /**
         * This is the equality comparison operator for the class
         * 
         * @param[in] otherInternetMessage
         *      This is the other InternetMessage to which to compare 
         *      this InternetMessage.
         * @return
         *      An indication of whether or not this InternetMessage is 
         *      equal to the other one
        */
       bool operator==(const InternetMessage& otherInternetMessage) const;
        /**
         * This is the inequality comparison operator for the class
         * 
         * @param[in] otherInternetMessage
         *      This is the other InternetMessage to which to compare 
         *      this InternetMessage.
         * @return
         *      An indication of whether or not this InternetMessage is 
         *      equal to the other one
        */
       bool operator!=(const InternetMessage& otherInternetMessage) const;
       /**
        * This method build the internet message by etermining the headers 
        * and body from the elements parsed from the given string
        * rendering of an internet message.
        * 
        * @param[in] internetMessageString
        *       This is the string rendering of the internet message to parce.
        * @return
        *       whether or not the internet message was parsed sucssefully 
        *       is returnd.
       */
       bool ParseFromString(const std::string& internetMessageString);

        /**
         * This method returns the collection of Headers elements of the Internet message.
         * 
         * @return
         *      The collection of Headers elements of the Internet message.
         * @retval ""
         *      An empty collection is returned if there is no "Header" element 
         *      in the Internet message.
        */
       Headers GetHeaders() const;

        /**
         * This method checks if there is a header with the given name
         * in the message.
         * 
         * @param[in] headerName
         *      This is the header name to check if it's present in the message.
         * 
         * @return 
         *      return an indication of whether or not an internet message have
         *      a Header.
        */
        bool HasHeader(const HeaderName& headerName) const;

        /**
         * This method returns the "Body" element of the Internet message.
         * 
         * @return
         *      The "Body" element of the Internet message.
         * 
         * @retval ""
         *      An empty string is returned if there is no "Body" element 
         *      in the Internet message.
        */
       std::string GetBody() const;


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

#endif /* INTERNET_MESSAGE_HPP */
