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

#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <map>
#include <cstdint>
#include <memory>
#include <variant>

#include "../test/mocks/ip_addr.h"
#include "../test/mocks/ip4_addr.h"
#include "../test/mocks/ip.h"

#include "defaults.h"
#include "tcp_session.h"

class TcpServer
{
public:
    static TcpServer &getInstance();
    void cleanup();

    bool startTcpServer(unsigned short port, void (*cb)(void *, TcpSession::TcpSessionPtr), void *obj);
    bool startTcpClient(ip_addr_t ipAddress, unsigned short port, void (*cb)(void *, TcpSession::TcpSessionPtr), void *obj);
    bool stopTcpServer();
    bool stopTcpClient(ip_addr_t ipAddress);
    void sessionConnected(void *arg);
    void sessionDisconnected(TcpSession::SessionId sessionId);
    std::size_t getSessionCount();
    TcpSession::TcpSessionPtr getSession(TcpSession::SessionId sessionId);
    void sessionDead(TcpSession::TcpSessionPtr);

    TcpServer();
    ~TcpServer();

private:
    static std::unique_ptr<TcpServer> instance_;
    TcpServer(const TcpServer &) = delete;
    TcpServer &operator=(const TcpServer &) = delete;
    TcpServer(TcpServer &&) = delete;
    TcpServer &operator=(TcpServer &&) = delete;
};

#endif // TCP_SERVER_H