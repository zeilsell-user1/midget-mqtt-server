#include <doctest.h>
#include "mqtt_connack_parser.h"

TEST_SUITE("MqttConnackParser")
{
    TEST_CASE("CONNACK successful Connection")
    {
        unsigned char bytes[] = {0x20, 0x02, 0x00, 0x00};
        std::vector<unsigned char> byteVector(bytes, bytes + sizeof(bytes) / sizeof(bytes[0]));
        MqttConnackParser parser = MqttConnackParser();
        MqttMessageParser::ParseResult result = parser.parseMessage(byteVector);
        REQUIRE_EQ(result, MqttMessageParser::ParseResult::Success);
        REQUIRE_EQ(parser.getConnackReturnCode(), MqttConnackParser::MqttConnackReturnCode::Success);
    }

    TEST_CASE("CONNACK unsuccessful Connection - unacceptable protocol version")
    {
        unsigned char bytes[] = {0x20, 0x02, 0x01, 0x84};
        std::vector<unsigned char> byteVector(bytes, bytes + sizeof(bytes) / sizeof(bytes[0]));
        MqttConnackParser parser = MqttConnackParser();
        MqttMessageParser::ParseResult result = parser.parseMessage(byteVector);
        REQUIRE_EQ(result, MqttMessageParser::ParseResult::Success);
        REQUIRE_EQ(parser.getConnackReturnCode(), MqttConnackParser::MqttConnackReturnCode::UnsupportedProtocolVersion);
    }

    TEST_CASE("CONNACK unsuccessful Connection - identifier rejected")
    {
        unsigned char bytes[] = {0x20, 0x02, 0x01, 0x85};
        std::vector<unsigned char> byteVector(bytes, bytes + sizeof(bytes) / sizeof(bytes[0]));
        MqttConnackParser parser = MqttConnackParser();
        MqttMessageParser::ParseResult result = parser.parseMessage(byteVector);
        REQUIRE_EQ(result, MqttMessageParser::ParseResult::Success);
        REQUIRE_EQ(parser.getConnackReturnCode(), MqttConnackParser::MqttConnackReturnCode::ClientIdentifierNotValid);
    }

    TEST_CASE("CONNACK unsuccessful Connection - server unavailable")
    {
        unsigned char bytes[] = {0x20, 0x02, 0x01, 0x88};
        std::vector<unsigned char> byteVector(bytes, bytes + sizeof(bytes) / sizeof(bytes[0]));
        MqttConnackParser parser = MqttConnackParser();
        MqttMessageParser::ParseResult result = parser.parseMessage(byteVector);
        REQUIRE_EQ(result, MqttMessageParser::ParseResult::Success);
        REQUIRE_EQ(parser.getConnackReturnCode(), MqttConnackParser::MqttConnackReturnCode::ServerUnavailable);
    }

    TEST_CASE("CONNACK unsuccessful Connection - bad username or password")
    {
        unsigned char bytes[] = {0x20, 0x02, 0x01, 0x86};
        std::vector<unsigned char> byteVector(bytes, bytes + sizeof(bytes) / sizeof(bytes[0]));
        MqttConnackParser parser = MqttConnackParser();
        MqttMessageParser::ParseResult result = parser.parseMessage(byteVector);
        REQUIRE_EQ(result, MqttMessageParser::ParseResult::Success);
        REQUIRE_EQ(parser.getConnackReturnCode(), MqttConnackParser::MqttConnackReturnCode::BadUserNameOrPassword);
    }

    TEST_CASE("CONNACK unsuccessful Connection - not authorised")
    {
        unsigned char bytes[] = {0x20, 0x02, 0x01, 0x87};
        std::vector<unsigned char> byteVector(bytes, bytes + sizeof(bytes) / sizeof(bytes[0]));
        MqttConnackParser parser = MqttConnackParser();
        MqttMessageParser::ParseResult result = parser.parseMessage(byteVector);
        REQUIRE_EQ(result, MqttMessageParser::ParseResult::Success);
        REQUIRE_EQ(parser.getConnackReturnCode(), MqttConnackParser::MqttConnackReturnCode::NotAuthorized);
    }

    TEST_CASE("invalid CONNACK - invalid remaining length")
    {
        unsigned char bytes[] = {0x20, 0x49, 0x00, 0x00};
        std::vector<unsigned char> byteVector(bytes, bytes + sizeof(bytes) / sizeof(bytes[0]));
        MqttConnackParser parser = MqttConnackParser();
        MqttMessageParser::ParseResult result = parser.parseMessage(byteVector);
        REQUIRE_EQ(result, MqttMessageParser::ParseResult::InvalidRemainingLength);
        REQUIRE_EQ(parser.getConnackReturnCode(), MqttConnackParser::MqttConnackReturnCode::InvalidReturnCode);
    }

    TEST_CASE("invalid CONNACK - missing connect acknowledge flags")
    {
        unsigned char bytes[] = {0x20, 0x02, 0xE0, 0x00};
        std::vector<unsigned char> byteVector(bytes, bytes + sizeof(bytes) / sizeof(bytes[0]));
        MqttConnackParser parser = MqttConnackParser();
        MqttMessageParser::ParseResult result = parser.parseMessage(byteVector);
        REQUIRE_EQ(result, MqttMessageParser::ParseResult::InvalidMessageStructure);
        REQUIRE_EQ(parser.getConnackReturnCode(), MqttConnackParser::MqttConnackReturnCode::InvalidReturnCode);
    }

    TEST_CASE("invalid CONNACK - invalid connect return code")
    {
        unsigned char bytes[] = {0x20, 0x02, 0x00, 0xFE};
        std::vector<unsigned char> byteVector(bytes, bytes + sizeof(bytes) / sizeof(bytes[0]));
        MqttConnackParser parser = MqttConnackParser();
        MqttMessageParser::ParseResult result = parser.parseMessage(byteVector);
        REQUIRE_EQ(result, MqttMessageParser::ParseResult::InvalidReturnCode);
        REQUIRE_EQ(parser.getConnackReturnCode(), MqttConnackParser::MqttConnackReturnCode::InvalidReturnCode);
    }
}