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

#include <vector>
#include <cstdint>
#include "string.h"
#include "defaults.h"
#include "mqtt_message_parser.h"

bool MqttMessageParser::parseProperties(const std::vector<unsigned char> &message, size_t &index)
{
    size_t currentIndex = index; // Use a local variable instead of reusing the parameter

    while (currentIndex < message.size()) 
    {
        MqttPropertyTypes propertyType;
        if (!parseVariableByteIntegerProperty(message, reinterpret_cast<unsigned long &>(propertyType), currentIndex))
        {
            return false; // Error: Unable to parse property type
        }

        switch (propertyType)
        {
        case MqttPropertyTypes::PayloadFormatIndicator:
        case MqttPropertyTypes::RequestProblemInformation:
        case MqttPropertyTypes::RequestResponseInformation:
        case MqttPropertyTypes::RetainAvailable:
        case MqttPropertyTypes::WildcardSubscriptionAvailable:
        case MqttPropertyTypes::SubscriptionIdentifierAvailable:
        case MqttPropertyTypes::SharedSubscriptionAvailable:
            // These properties are of type 'Byte'
            {
                unsigned char byteValue;
                if (!parseByteProperty(message, byteValue, currentIndex))
                {
                    return false; // Error: Unable to parse Byte property
                }
                // Process 'byteValue' as needed... TODO
                break;
            }
        case MqttPropertyTypes::MessageExpiryInterval:
        case MqttPropertyTypes::WillDelayInterval:
        case MqttPropertyTypes::MaximumPacketSize:
        case MqttPropertyTypes::SessionExpiryInterval:
            // These properties are of type 'Four Byte Integer'
            {
                unsigned long integerValue;
                if (!parseFourByteIntegerProperty(message, integerValue, index))
                {
                    return false; // Error: Unable to parse Four Byte Integer property
                }
                // Process 'integerValue' as needed... TODO
                break;
            }
        case MqttPropertyTypes::TopicAlias:
        case MqttPropertyTypes::TopicAliasMaximum:
        case MqttPropertyTypes::MaximumQoS:
        case MqttPropertyTypes::ReceiveMaximum:
        case MqttPropertyTypes::ServerKeepAlive:
            // These properties are of type 'Two Byte Integer'
            {
                unsigned short integerValue;
                if (!parseTwoByteIntegerProperty(message, integerValue, index))
                {
                    return false; // Error: Unable to parse Two Byte Integer property
                }
                // Process 'integerValue' as needed... TODO
                break;
            }
        case MqttPropertyTypes::ContentType:
        case MqttPropertyTypes::ResponseTopic:
        case MqttPropertyTypes::AuthenticationMethod:
        case MqttPropertyTypes::ResponseInformation:
        case MqttPropertyTypes::ServerReference:
        case MqttPropertyTypes::ReasonString:
        case MqttPropertyTypes::AssignedClientIdentifier:
        case MqttPropertyTypes::UserProperty:
            // These properties are of type 'UTF-8 Encoded String'
            {
                char *stringValue;
                size_t stringLength;
                if (!parseUtf8StringProperty(message, stringValue, stringLength, index))
                {
                    return false; // Error: Unable to parse UTF-8 Encoded String property
                }
                // Process 'stringValue' and 'stringLength' as needed... TODO
                break;
            }
        case MqttPropertyTypes::CorrelationData:
        case MqttPropertyTypes::AuthenticationData:
            // These properties are of type 'Binary Data'
            {
                std::vector<unsigned char> binaryValue;
                if (!parseBinaryDataProperty(message, binaryValue, index))
                {
                    return false; // Error: Unable to parse Binary Data property
                }
                // Process 'binaryValue' as needed... TODO
                break;
            }
        case MqttPropertyTypes::SubscriptionIdentifier:
            // This property is of type 'Variable Byte Integer'
            {
                unsigned long integerValue;
                if (!parseVariableByteIntegerProperty(message, integerValue, index))
                {
                    return false; // Error: Unable to parse Variable Byte Integer property
                }
                // Process 'integerValue' as needed... TODO
                break;
            }
        default:
            // Unknown property type
            return false;
        }
    }

    index = currentIndex; // Update the index parameter with the new value
    return true;          // Successfully parsed all properties
}

bool MqttMessageParser::parseByteProperty(const std::vector<unsigned char> &data, unsigned char &result, size_t &index)
{
    if (index < data.size())
    {
        result = data[index++];
        return true;
    }
    return false;
}

bool MqttMessageParser::parseFourByteIntegerProperty(const std::vector<unsigned char> &data, unsigned long &result, size_t &index)
{
    if (index + 3 < data.size())
    {
        result = (static_cast<unsigned long>(data[index]) << 24) |
                 (static_cast<unsigned long>(data[index + 1]) << 16) |
                 (static_cast<unsigned long>(data[index + 2]) << 8) |
                 static_cast<unsigned long>(data[index + 3]);
        index += 4;
        return true;
    }
    return false;
}

bool MqttMessageParser::parseUtf8StringProperty(const std::vector<unsigned char> &message, char *value, size_t &index, size_t maxLength)
{
    // Ensure that the index is within bounds
    if (index >= message.size())
    {
        return false;
    }

    // Get the length of the UTF-8 string
    size_t length = message[index++];

    // Check if the length exceeds the maximum allowed
    if (length > maxLength)
    {
        return false;
    }

    // Ensure that there is enough data in the message
    if (index + length > message.size())
    {
        return false;
    }

    // Copy the UTF-8 string to the provided buffer
    memcpy(value, &message[index], length);

    // Null-terminate the C string
    value[length] = '\0';

    // Update the index
    index += length;

    return true;
}

bool MqttMessageParser::parseBinaryDataProperty(const std::vector<unsigned char> &data, std::vector<unsigned char> &result, size_t &index)
{
    if (index + 1 < data.size())
    {
        unsigned short binLen = (static_cast<unsigned short>(data[index]) << 8) | static_cast<unsigned short>(data[index + 1]);
        index += 2;

        if (index + binLen <= data.size())
        {
            result.assign(data.begin() + index, data.begin() + index + binLen);
            index += binLen;
            return true;
        }
    }
    return false;
}

bool MqttMessageParser::parseVariableByteIntegerProperty(const std::vector<unsigned char> &data, unsigned long &result, size_t &index)
{
    result = 0;
    unsigned char shift = 0;
    while (index < data.size())
    {
        unsigned char encodedByte = data[index++];
        result |= static_cast<unsigned long>(encodedByte & 0x7F) << shift;
        if ((encodedByte & 0x80) == 0)
        {
            return true;
        }
        shift += 7;
    }
    return false;
}

bool MqttMessageParser::parseTwoByteIntegerProperty(const std::vector<unsigned char> &data, unsigned short &result, size_t &index)
{
    if (index + 1 < data.size())
    {
        result = (static_cast<unsigned short>(data[index]) << 8) | static_cast<unsigned short>(data[index + 1]);
        index += 2;
        return true;
    }
    return false;
}
