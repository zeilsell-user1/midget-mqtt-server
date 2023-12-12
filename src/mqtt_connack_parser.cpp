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

// int main()
// {
//     MqttConnackParser parser;

//     // Example: Invalid CONNACK
//     std::vector<uint8_t> invalidConnack = {0x20, 0x02, 0x01, 0x02};
//     MqttConnackParser::ParseResult result = parser.parse(invalidConnack);

//     switch (result)
//     {
//     case MqttConnackParser::ParseResult::Success:
//         // Handle successful parse
//         break;
//     case MqttConnackParser::ParseResult::IncompleteData:
//         // Handle incomplete data
//         break;
//     case MqttConnackParser::ParseResult::InvalidPacketType:
//         // Handle invalid packet type
//         break;
//     case MqttConnackParser::ParseResult::InvalidRemainingLength:
//         // Handle invalid remaining length
//         break;
//     case MqttConnackParser::ParseResult::InvalidFlags:
//         // Handle invalid flags
//         break;
//     case MqttConnackParser::ParseResult::InvalidReturnCode:
//         // Handle invalid return code
//         break;
//     }
//     return 0;
// }

#include <type_traits>
#include "mqtt_connack_parser.h"

MqttConnackParser::MqttConnackParser() {}

MqttMessageParser::ParseResult MqttConnackParser::parseMessage(const std::vector<unsigned char> &connackMessage)
{
    MQTT_INFO("Entered MqttConnackParser");
    MQTT_INFO("Received message length is %d", connackMessage.size());
    connackMessage_ = connackMessage;
    currentIndex_ = 0;
    connackReturnCode_ = MqttConnackReturnCode::InvalidReturnCode;

    if (parseFixedHeader() == false)
    {
        MQTT_WARNING("Failed to parse fixed header");
        return ParseResult::InvalidMessageStructure;
    }

    // Check if there is enough data to match the remaining length
    if ((remainingLength_) != connackMessage_.size() - currentIndex_)
    {
        MQTT_WARNING("Not enough data in the message to match remaining length");
        return ParseResult::InvalidRemainingLength;
    }

    return parseVariableHeader();
}

MqttConnackParser::MqttConnackReturnCode MqttConnackParser::getConnackReturnCode() const
{
    return connackReturnCode_;
}

bool MqttConnackParser::parseFixedHeader()
{
    MQTT_INFO("enter parseFixedHeader, currentIndex_ = %d", currentIndex_);
    // The first byte of the message is the fixed header
    fixedHeader_ = connackMessage_[currentIndex_++];
    MQTT_INFO("*** fixedHeader_ = 0x%x", fixedHeader_);
    MQTT_INFO("currentIndex_ = %d", currentIndex_);

    if (fixedHeader_ != 0x20)
    {
        MQTT_WARNING("wrong value in the fixed header");
        return false;
    }

    remainingLength_ = parseRemainingLength();
    MQTT_INFO("remainingLength_ = %d", remainingLength_);
    return true;
}

MqttMessageParser::ParseResult MqttConnackParser::parseVariableHeader()
{
    // The next bytes are part of the variable header
    if (!parseConnectAcknowledgeFlags())
    {
        MQTT_WARNING("Failed to parse connect acknowledge flags");
        return ParseResult::InvalidMessageStructure;
    }

    if (!parseConnackReturnCode())
    {
        MQTT_WARNING("Failed to parse connack return code");
        return ParseResult::InvalidReturnCode;
    }

    if (!parseProperties())
    {
        MQTT_WARNING("Failed to parse properties");
        return ParseResult::InvalidMessageStructure;
    }

    return ParseResult::Success;
}

int MqttConnackParser::parseRemainingLength()
{
    MQTT_INFO("enter parseRemainingLength, currentIndex_ = %d", currentIndex_);
    MQTT_INFO("*** reaminaing length byte in header = 0x%x", connackMessage_[currentIndex_]);

    int multiplier = 1;
    int value = 0;
    unsigned char byte;

    do
    {
        byte = connackMessage_[currentIndex_++];
        value += (byte & 127) * multiplier;
        multiplier *= 128;
    } while ((byte & 128) != 0);

    MQTT_INFO("leave parseRemainingLength, currentIndex_ = %d", currentIndex_);
    return value;
}

bool MqttConnackParser::parseConnectAcknowledgeFlags()
{
    MQTT_INFO("enter parseConnectAcknowledgeFlags, currentIndex_ = %d", currentIndex_);
    acknowledgeFlags_ = connackMessage_[currentIndex_++];
    MQTT_INFO("*** acknowledgeFlags_ = 0x%x", acknowledgeFlags_);
    MQTT_INFO("currentIndex_ = %d", currentIndex_);

    if ((acknowledgeFlags_ & 0xFE) != 0x00)
    {
        MQTT_WARNING("Invalid acknowledge flags )x%x", acknowledgeFlags_);
        return false;
    }

    sessionPresent_ = (acknowledgeFlags_ & 0x01);
    return true;
}

bool MqttConnackParser::parseConnackReturnCode()
{
    connackReturnCode_ = static_cast<MqttConnackReturnCode>(connackMessage_[currentIndex_++]);

    switch (connackReturnCode_)
    {
    case MqttConnackReturnCode::Success:
    case MqttConnackReturnCode::UnspecifiedError:
    case MqttConnackReturnCode::MalformedPacket:
    case MqttConnackReturnCode::ProtocolError:
    case MqttConnackReturnCode::ImplementationSpecificError:
    case MqttConnackReturnCode::UnsupportedProtocolVersion:
    case MqttConnackReturnCode::ClientIdentifierNotValid:
    case MqttConnackReturnCode::BadUserNameOrPassword:
    case MqttConnackReturnCode::NotAuthorized:
    case MqttConnackReturnCode::ServerUnavailable:
    case MqttConnackReturnCode::ServerBusy:
    case MqttConnackReturnCode::Banned:
    case MqttConnackReturnCode::BadAuthenticationMethod:
    case MqttConnackReturnCode::TopicNameInvalid:
    case MqttConnackReturnCode::PacketTooLarge:
    case MqttConnackReturnCode::QuotaExceeded:
    case MqttConnackReturnCode::PayloadFormatInvalid:
    case MqttConnackReturnCode::RetainNotSupported:
    case MqttConnackReturnCode::QosNotSupported:
    case MqttConnackReturnCode::UseAnotherServer:
    case MqttConnackReturnCode::ServerMoved:
    case MqttConnackReturnCode::ConnectionRateExceeded:
        MQTT_INFO("Valid connack return code: 0x%x", connackReturnCode_);
        return true;

    default:
        MQTT_WARNING("Invalid connack return code: 0x%x", connackReturnCode_);
        connackReturnCode_ = MqttConnackReturnCode::InvalidReturnCode;
        return false;
    }
}

bool MqttConnackParser::parseProperties()
{

    MQTT_INFO("enter parseProperties, currentIndex_ = %d", currentIndex_);

    size_t index = currentIndex_;
    return MqttMessageParser::parseProperties(connackMessage_, index);
}
