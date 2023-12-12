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

#ifndef TCP_SESSION_H
#define TCP_SESSION_H

#include <memory>    // For std::shared_ptr, std::unique_ptr
#include <stdexcept> // For std::runtime_error

#include "../test/mocks/ip_addr.h"
#include "../test/mocks/ip4_addr.h"
#include "../test/mocks/ip.h"

class TcpSession : public std::enable_shared_from_this<TcpSession>
{
public:
    using TcpSessionPtr = std::shared_ptr<TcpSession>;
    using SessionId = std::size_t;

    enum sendResult
    {
        SEND_OK,
        RETRY,
        FAILED_ABORTED
    };

    bool isSessionValid();
    SessionId getSessionId();
    void disconnectSession();
    sendResult sendMessage(unsigned char *pData, unsigned short len);

    bool registerSessionDisconnectedCb(void (*cb)(void *obj, TcpSessionPtr session), void *obj);
    bool registerSessionReconnectCb(void (*cb)(void *obj, signed char err, TcpSessionPtr session), void *obj);
    bool registerIncomingMessageCb(void (*cb)(void *obj, char *pData, unsigned short len, TcpSessionPtr session), void *obj);
    bool registerMessageSentCb(void (*cb)(void *obj, TcpSessionPtr session), void *obj);

    static ip_addr_t convertIpAddress(unsigned char *);
    static SessionId createUniqueIdentifier(const ip_addr_t &ipAddress, int port);

    ~TcpSession();

    TcpSession(const TcpSession &) = default;
    TcpSession &operator=(const TcpSession &) = default;
    TcpSession(TcpSession &&) = default;
    TcpSession &operator=(TcpSession &&) = default;

private:
    TcpSession() = default;

};

#endif // TCP_SESSION_H