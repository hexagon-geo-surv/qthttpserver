// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QHttpServerHttp1ProtocolHandler_H
#define QHttpServerHttp1ProtocolHandler_H

#include <QtHttpServer/qthttpserverglobal.h>
#include <QtHttpServer/qhttpserverrequest.h>
#include <QtHttpServer/private/qhttpserverstream_p.h>
#include <QtHttpServer/private/qhttpserverrequestfilter_p.h>

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of QHttpServer. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

QT_BEGIN_NAMESPACE

class QTcpSocket;
class QAbstractHttpServer;
#if QT_CONFIG(localserver)
class QLocalSocket;
#endif

class QHttpServerHttp1ProtocolHandler : public QHttpServerStream
{
    Q_OBJECT

    friend class QAbstractHttpServerPrivate;
    friend class QHttpServerResponder;

private:
    QHttpServerHttp1ProtocolHandler(QAbstractHttpServer *server,
                                    QIODevice *socket,
                                    QHttpServerRequestFilter *filter);

    void responderDestroyed() final;
    void startHandlingRequest() final;
    void socketDisconnected() final;

    void handleReadyRead();

    void write(const QByteArray &body, const QHttpHeaders &headers,
               QHttpServerResponder::StatusCode status, quint32 streamId) final;
    void write(QHttpServerResponder::StatusCode status, quint32 streamId) final;
    void write(QIODevice *data, const QHttpHeaders &headers,
               QHttpServerResponder::StatusCode status, quint32 streamId) final;
    void writeBeginChunked(const QHttpHeaders &headers,
                           QHttpServerResponder::StatusCode status,
                           quint32 streamId) final;
    void writeChunk(const QByteArray &body, quint32 streamId) final;
    void writeEndChunked(const QByteArray &data,
                         const QHttpHeaders &trailers,
                         quint32 streamId) final;

    void writeStatusAndHeaders(QHttpServerResponder::StatusCode status,
                               const QHttpHeaders &headers);
    void writeHeader(const QByteArray &key, const QByteArray &value);
    void write(const QByteArray &data);
    void write(const char *body, qint64 size);

    QAbstractHttpServer *server;
    QIODevice *socket;
    QTcpSocket *tcpSocket;
#if QT_CONFIG(localserver)
    QLocalSocket *localSocket;
#endif
    QHttpServerRequestFilter *m_filter;

    enum class TransferState {
        Ready,
        HeadersSent,
        ChunkedTransferBegun
    } state = TransferState::Ready;

    QHttpServerRequest request;

   // To avoid destroying the object when socket object is destroyed while
   // a request is still being handled.
    bool handlingRequest = false;
    bool protocolChanged = false;
};

QT_END_NAMESPACE

#endif // QHttpServerHttp1ProtocolHandler_H
