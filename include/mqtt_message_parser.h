/*******************************************************************************
 * Copyright (c) 2023 George Consultants Ltd.
 * richard.john.george.3@gmail.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the “Software”),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *******************************************************************************/

#ifndef __MQTT_MESSAGE_PARSER_H__
#define __MQTT_MESSAGE_PARSER_H__

#include <vector>
#include <type_traits>
#include "defaults.h"

class MqttMessageParser
{
public:
    enum class ParseResult
    {
        Success,
        InvalidPacketType,
        IncompleteData,
        InvalidRemainingLength,
        InvalidSessionPresent,
        InvalidMessageStructure,
        InvalidReturnCode
    };

    enum class MqttPropertyTypes : unsigned char
    {
        PayloadFormatIndicator = 0x01,          // Byte PUBLISH, Will Properties
        MessageExpiryInterval = 0x02,           // Four Byte Integer PUBLISH, Will Properties
        ContentType = 0x03,                     // UTF-8 Encoded String PUBLISH, Will Properties
        ResponseTopic = 0x08,                   // UTF-8 Encoded String PUBLISH, Will Properties
        CorrelationData = 0x09,                 // Binary Data PUBLISH, Will Properties
        SubscriptionIdentifier = 0x0B,          // Variable Byte Integer PUBLISH, SUBSCRIBE
        SessionExpiryInterval = 0x11,           // Four Byte Integer CONNECT, CONNACK, DISCONNECT
        AssignedClientIdentifier = 0x12,        // UTF-8 Encoded String CONNACK
        ServerKeepAlive = 0x13,                 // Two Byte Integer CONNACK
        AuthenticationMethod = 0x15,            // UTF-8 Encoded String CONNECT, CONNACK, AUTH
        AuthenticationData = 0x16,              // Binary Data CONNECT, CONNACK, AUTH
        RequestProblemInformation = 0x17,       // Byte CONNECT
        WillDelayInterval = 0x18,               // Four Byte Integer Will Properties
        RequestResponseInformation = 0x19,      // Byte CONNECT
        ResponseInformation = 0x1A,             // UTF-8 Encoded String CONNACK
        ServerReference = 0x1C,                 // UTF-8 Encoded String CONNACK, DISCONNECT
        ReasonString = 0x1F,                    // UTF-8 Encoded String CONNACK, PUBACK, PUBREC, PUBREL, PUBCOMP, SUBACK, UNSUBACK, DISCONNECT, AUTH
        ReceiveMaximum = 0x21,                  // Two Byte Integer CONNECT, CONNACK
        TopicAliasMaximum = 0x22,               // Two Byte Integer CONNECT, CONNACK
        TopicAlias = 0x23,                      // Two Byte Integer PUBLISH
        MaximumQoS = 0x24,                      // Byte CONNACK
        RetainAvailable = 0x25,                 // Byte CONNACK
        UserProperty = 0x26,                    // UTF-8 String Pair CONNECT, CONNACK, PUBLISH, Will Properties, PUBACK, PUBREC, PUBREL, PUBCOMP, SUBSCRIBE, SUBACK, UNSUBSCRIBE, UNSUBACK, DISCONNECT, AUTH
        MaximumPacketSize = 0x27,               // Four Byte Integer CONNECT, CONNACK
        WildcardSubscriptionAvailable = 0x28,   // Byte CONNACK
        SubscriptionIdentifierAvailable = 0x29, // Byte CONNACK
        SharedSubscriptionAvailable = 0x2A      // Byte CONNACK
    };

    virtual ParseResult parseMessage(const std::vector<unsigned char> &message) = 0;
    static bool parseProperties(const std::vector<unsigned char> &message, size_t &index);

private:
    static bool parseByteProperty(const std::vector<unsigned char> &message, unsigned char &value, size_t &index);
    static bool parseFourByteIntegerProperty(const std::vector<unsigned char> &message, unsigned long &value, size_t &index);
    static bool parseTwoByteIntegerProperty(const std::vector<unsigned char> &message, unsigned short &value, size_t &index);
    static bool parseUtf8StringProperty(const std::vector<unsigned char> &message, char *value, size_t &index, size_t maxLength);
    static bool parseBinaryDataProperty(const std::vector<unsigned char> &message, std::vector<unsigned char> &value, size_t &index);
    static bool parseVariableByteIntegerProperty(const std::vector<unsigned char> &message, unsigned long &value, size_t &index);
};

#endif // __MQTT_MESSAGE_PARSER_H__
