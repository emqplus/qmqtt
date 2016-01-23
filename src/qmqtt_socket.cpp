/*
 * qmqtt_socket.cpp - qmqtt socket
 *
 * Copyright (c) 2013  Ery Lee <ery.lee at gmail dot com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of mqttc nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */
#include "qmqtt_socket.h"
#include <QTcpSocket>

QMQTT::Socket::Socket(QObject* parent)
    : SocketInterface(parent)
    , _socket(new QTcpSocket)
{
    connect(_socket.data(), &QTcpSocket::stateChanged, this, &Socket::onStateChanged);
    connect(_socket.data(), &QTcpSocket::connected,    this, &SocketInterface::connected);
    connect(_socket.data(), &QTcpSocket::disconnected, this, &SocketInterface::disconnected);
    connect(_socket.data(), &QTcpSocket::readyRead,    this, &SocketInterface::readyRead);
    connect(_socket.data(),
            static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error),
            this,
            static_cast<void (SocketInterface::*)(QAbstractSocket::SocketError)>(&SocketInterface::error));\
}

QMQTT::Socket::~Socket()
{
}

void QMQTT::Socket::connectToHost(const QHostAddress& address, quint16 port)
{
    return _socket->connectToHost(address, port);
}

void QMQTT::Socket::disconnectFromHost()
{
    _socket->disconnectFromHost();
}

QAbstractSocket::SocketState QMQTT::Socket::state() const
{
    return _socket->state();
}

bool QMQTT::Socket::atEnd() const
{
    return _socket->atEnd();
}


bool QMQTT::Socket::waitForBytesWritten(int msecs)
{
    return _socket->waitForBytesWritten(msecs);
}

bool QMQTT::Socket::waitForReadyRead(int msecs)
{
    return _socket->waitForReadyRead(msecs);
}

QAbstractSocket::SocketError QMQTT::Socket::error() const
{
    return _socket->error();
}

// QIODevice demands
qint64 QMQTT::Socket::readData(char* data, qint64 maxlen)
{
    return _socket->read(data, maxlen);
}

qint64 QMQTT::Socket::writeData(const char* data, qint64 len)
{
    return _socket->write(data, len);
}

void QMQTT::Socket::onStateChanged(QAbstractSocket::SocketState state)
{
    Q_UNUSED(state);
    if(openMode() != _socket->openMode())
    {
        setOpenMode(_socket->openMode());
    }
}
