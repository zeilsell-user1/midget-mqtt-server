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

#ifndef _MQTT_SERVER_H_
#define _MQTT_SERVER_H_

#include <memory>

#ifdef ESP8266
#include <lwip/ip.h>
#else
#include "../test/test_mqtt_server/ip_addr.h"
#include "../test/test_mqtt_server/ip4_addr.h"
#include "../test/test_mqtt_server/ip.h"
#endif

#include "tcp_server.h"
#include "mqtt_session.h"

// Message Queuing Telemetry Transport (MQTT) is a lightweight and open messaging protocol 
// designed for small sensors and mobile devices with high-latency or unreliable networks. 
// MQTT is often used in Internet of Things (IoT) scenarios and other applications where 
// low bandwidth and high latency are common challenges. The protocol operates on a 
// publish/subscribe model, where devices can publish messages to specific topics, and other 
// devices can subscribe to receive messages from those topics.The MQTT Server can run as 
// either the client or the server. It provides the managment of MQTT connections, 
// processes the received incoming messages and executes the appropriate actions.
class MqttServer
{
public:
  static MqttServer &getInstance();
  void cleanup();

  bool startMqttClient(ip_addr_t ipAddress, unsigned short port);
  bool startMqttServer(unsigned short portno);
  bool stopMqttServer();
  bool stopMqttClient();
  void sessionConnected();
  void disconnectSession(MqttSession::SessionId sessionId);
  void sessionDisconnected(MqttSession::SessionId sessionId);
  std::size_t getSessionCount();
  MqttSession::MqttSessionPtr getSession(MqttSession::SessionId sessionId);

  // A drawback of using the RAII (Resource Acquisition Is Initialization) principle is that
  // shared_ptr and unique_ptr both need to have access to the constructor and destructor for
  // the class. Unfortunately this means they need to be public, which sucks. Please DO NOT
  // call MqttServer directly, use getInstance() to get a pointer to the instance

  MqttServer();
  ~MqttServer();

  void handleTcpSessionConnect(std::shared_ptr<TcpSession> tcpSession);

private:
    MqttServer(const MqttServer &) = delete;
    MqttServer &operator=(const MqttServer &) = delete;
    MqttServer(MqttServer &&) = delete;
    MqttServer &operator=(MqttServer &&) = delete;

    MqttSession::MqttSessionPtr createMqttSession(ip_addr_t ipAddress, unsigned short port);
    bool addSession(MqttSession::SessionId sessionId, const MqttSession::MqttSessionPtr session);
    void removeSession(MqttSession::SessionId sessionId);
    void removeAllSessions();
    MqttSession::MqttSessionPtr getSession(MqttSession::SessionId sessionId) const;

private:
  struct MapSessions
  {
    bool mappingValid;
    std::shared_ptr<TcpSession> tcpSession;
    std::unique_ptr<MqttSession> mqttSession;
  };

  MapSessions sessionMapping_[MAX_MQTT_SESSIONS];
  ip_addr_t ipAddress_;
  unsigned short port_;
};

#endif /* _MQTT_SERVER_H_ */

// #include "mqtt_msg.h"
// #include "mqtt_topic.h"

// uint16_t MQTT_server_countClientCon();
// char *MQTT_server_getClientId(uint16_t index);
// struct espconn *MQTT_server_getClientPcon(uint16_t index);

// bool MQTT_server_start(uint16_t portno, uint16_t max_subscriptions, uint16_t max_retained_topics);
// void MQTT_server_onConnect(MqttConnectCallback connectCb);
// void MQTT_server_onDisconnect(MqttDisconnectCallback connectCb);
// void MQTT_server_onData(MqttDataCallback dataCb);
// bool MQTT_local_publish(uint8_t *topic, uint8_t *data, uint16_t data_length, uint8_t qos, uint8_t retain);
// bool MQTT_local_subscribe(uint8_t *topic, uint8_t qos);
// bool MQTT_local_unsubscribe(uint8_t *topic);
