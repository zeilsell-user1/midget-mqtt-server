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

#ifndef MQTT_SESSION_H
#define MQTT_SESSION_H

#include <memory>    // For std::shared_ptr, std::unique_ptr
#include <stdexcept> // For std::runtime_error

#ifdef NATIVE_BUILD
#include "../test/mocks/ip_addr.h"
#include "../test/mocks/ip4_addr.h"
#include "../test/mocks/ip.h"
#include "../test/mocks/tcp_session.h"
#else
#include <lwip/ip.h>
#include "tcp_session.h"
#endif

#include "defaults.h"
#include "mqtt_topic.h"
#include "mqtt_message.h"

// In the context of MQTT (Message Queuing Telemetry Transport), a "TCP session"
// usually encompasses the entire lifespan of a MQTT connection, from its
// establishment to its termination.
class MqttSession : public std::enable_shared_from_this<MqttSession>
{
public:
  using MqttSessionPtr = std::shared_ptr<MqttSession>;
  using SessionId = std::size_t;

  // A drawback of using the RAII (Resource Acquisition Is Initialization) principle is that
  // shared_ptr and unique_ptr both need to have access to the constructor and destructor for
  // the class. Unfortunately this means they need to be public, which sucks. Please DO NOT
  // call TcpSession directly, a session is created and managed by TcpServer

  MqttSession() = default;
  MqttSession(TcpSession::TcpSessionPtr tcpSession);
  ~MqttSession() = default;

  // In modern C++, it's generally recommended to follow the Rule of Three (or Rule of Five).
  // Since you have a custom destructor in MqttSession, it's good practice to also define or
  // delete the copy constructor, copy assignment operator, move constructor, and move
  // assignment operator. Not technically needed, but added anyway as good proactice.

  MqttSession(const MqttSession &) = default;
  MqttSession &operator=(const MqttSession &) = default;
  MqttSession(MqttSession &&) = default;
  MqttSession &operator=(MqttSession &&) = default;

  void setSessionFalse();
  bool isSessionValid();
  MqttSessionPtr getMqttSession();

  void handleTcpDisconnect(TcpSession::TcpSessionPtr tcpSession);
  void handleTcpReconnect(signed char err, TcpSession::TcpSessionPtr tcpSession);
  void handleTcpMessageSent(TcpSession::TcpSessionPtr tcpSession);
  void handleTcpIncomingMessage(TcpSession::TcpSessionPtr tcpSession, char *pdata, unsigned short len);

private: // state machine for the MQTT session
  void WaitForConnect_HandleMsg(MqttMessage msg);
  void Connected_HandleMsg(MqttMessage msg);
  void WaitForPubRel_HandleMsg(MqttMessage msg);
  void Disconnected_HandleMsg(MqttMessage msg);

private: // utility methods
  void print_topic(MqttTopic *topic) const;
  bool publish_topic(MqttTopic *topic, unsigned char *data, unsigned short data_len) const;

private:
  bool sessionValid_;
  TcpSession::TcpSessionPtr tcpSession_;
  unsigned char will_qos_;
  int will_retain_;
  int clean_session_;
  unsigned char clientId_[23];
  unsigned char IPAddress_[4];
  unsigned long sessionExpiryIntervalTimeout_;
};

#endif /* MQTT_SESSION_H */