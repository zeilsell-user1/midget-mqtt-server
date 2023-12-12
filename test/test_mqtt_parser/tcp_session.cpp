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

#include <string>
#include "../test/mocks/tcp_server.h"
#include "../test/mocks/tcp_session.h"

/*******************************************************************************
 * Class Implemenation - public
 *******************************************************************************/


TcpSession::~TcpSession()
{
}

uintptr_t TcpSession::getSessionId()
{
    return 1;
}

bool TcpSession::isSessionValid()
{
    return true;
}

void TcpSession::disconnectSession()
{
}

TcpSession::sendResult TcpSession::sendMessage(unsigned char *pData, unsigned short len)
{
    // return SEND_OK;
    // return FAILED_ABORTED;
    // return RETRY;

    return SEND_OK;
}

// Register the callback listener and the callbacls

bool TcpSession::registerSessionDisconnectedCb(void (*cb)(void *, TcpSessionPtr session), void *obj)
{
    return true;
}

bool TcpSession::registerSessionReconnectCb(void (*cb)(void *, signed char err, TcpSessionPtr session), void *obj)
{
    return true;
}

bool TcpSession::registerIncomingMessageCb(void (*cb)(void *, char *pdata, unsigned short len, TcpSessionPtr session), void *obj)
{
    return true;
}

bool TcpSession::registerMessageSentCb(void (*cb)(void *, TcpSessionPtr session), void *obj)
{
    return true;
}

// stiatic utility methods

ip_addr_t TcpSession::convertIpAddress(unsigned char *ipAddr)
{
    ip_addr_t ipAddress;
    IP4_ADDR(&ipAddress, ipAddr[0], ipAddr[1], ipAddr[2], ipAddr[3]);
    return ipAddress;
}

TcpSession::SessionId TcpSession::createUniqueIdentifier(const ip_addr_t &ipAddress, int port)
{
    // Convert IP address to string
    std::string ipString = std::to_string(ipAddress.addr);

    // Concatenate IP address and port as a string
    std::string combinedString = std::string(ipString) + ":" + std::to_string(port);

    // Hash the combined string to produce a unique identifier
    std::hash<std::string> hasher;
    return hasher(combinedString);
}