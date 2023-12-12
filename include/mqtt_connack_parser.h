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
#include "mqtt_message_parser.h"
#include "mqtt_utilties.h"

class MqttConnackParser : public MqttMessageParser
{
public:
    enum class MqttConnackReturnCode : unsigned char
    {
        Success = 0x00,                     // The Connection is accepted.
        UnspecifiedError = 0x80,            // The Server does not wish to reveal the reason for the failure, or none of the other Reason Codes apply.
        MalformedPacket = 0x81,             // Data within the CONNECT packet could not be correctly parsed.
        ProtocolError = 0x82,               // Data in the CONNECT packet does not conform to this specification.
        ImplementationSpecificError = 0x83, // The CONNECT is valid but is not accepted by this Server.
        UnsupportedProtocolVersion = 0x84,  // The Server does not support the version of the MQTT protocol requested by the Client.
        ClientIdentifierNotValid = 0x85,    // The Client Identifier is a valid string but is not allowed by the Server.
        BadUserNameOrPassword = 0x86,       // The Server does not accept the User Name or Password specified by the Client
        NotAuthorized = 0x87,               // The Client is not authorized to connect.
        ServerUnavailable = 0x88,           // The MQTT Server is not available.
        ServerBusy = 0x89,                  // The Server is busy. Try again later.
        Banned = 0x8A,                      // This Client has been banned by administrative action. Contact the server administrator.
        BadAuthenticationMethod = 0x8C,     // The authentication method is not supported or does not match the authentication method currently in use.
        TopicNameInvalid = 0x90,            // The Will Topic Name is not malformed, but is not accepted by this Server.
        PacketTooLarge = 0x95,              // The CONNECT packet exceeded the maximum permissible size.
        QuotaExceeded = 0x97,               // An implementation or administrative imposed limit has been exceeded.
        PayloadFormatInvalid = 0x99,        // The Will Payload does not match the specified Payload Format Indicator.
        RetainNotSupported = 0x9A,          // The Server does not support retained messages, and Will Retain was set to 1.
        QosNotSupported = 0x9B,             // The Server does not support the QoS set in Will QoS.
        UseAnotherServer = 0x9C,            // The Client should temporarily use another server.
        ServerMoved = 0x9D,                 // The Client should permanently use another server.
        ConnectionRateExceeded = 0x9F,      // The connection rate limit has been exceeded.
        InvalidReturnCode = 0xFE            // the return code was unrecognised

    };

    MqttConnackParser();
    ParseResult parseMessage(const std::vector<unsigned char> &connackMessage);
    MqttConnackReturnCode getConnackReturnCode() const;

private:
    bool parseFixedHeader();
    ParseResult parseVariableHeader();
    int parseRemainingLength();
    bool parseConnectAcknowledgeFlags();
    bool parseConnackReturnCode();
    bool parseProperties();

private:
    std::vector<unsigned char> connackMessage_;
    int currentIndex_;
    unsigned char fixedHeader_;
    unsigned char acknowledgeFlags_;
    int remainingLength_;
    bool sessionPresent_;
    MqttConnackReturnCode connackReturnCode_;
};
