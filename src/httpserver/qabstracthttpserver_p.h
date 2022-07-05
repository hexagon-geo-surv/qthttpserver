// Copyright (C) 2019 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QABSTRACTHTTPSERVER_P_H
#define QABSTRACTHTTPSERVER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of QHttpServer. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#include <QtHttpServer/qabstracthttpserver.h>
#include <QtHttpServer/qthttpserverglobal.h>

#include <private/qobject_p.h>

#if defined(QT_WEBSOCKETS_LIB)
#include <QtWebSockets/qwebsocketserver.h>
#endif // defined(QT_WEBSOCKETS_LIB)

#if QT_CONFIG(ssl)
#include <QtNetwork/qsslconfiguration.h>
#endif

QT_BEGIN_NAMESPACE

class QHttpServerRequest;

class Q_HTTPSERVER_EXPORT QAbstractHttpServerPrivate: public QObjectPrivate
{
public:
    Q_DECLARE_PUBLIC(QAbstractHttpServer)

    QAbstractHttpServerPrivate();

#if defined(QT_WEBSOCKETS_LIB)
    QWebSocketServer websocketServer {
        QLatin1StringView("QtHttpServer"),
        QWebSocketServer::NonSecureMode
    };
#endif // defined(QT_WEBSOCKETS_LIB)

    void handleNewConnections();
    void handleReadyRead(QTcpSocket *socket,
                         QHttpServerRequest *request);

#if QT_CONFIG(ssl)
    QSslConfiguration sslConfiguration;
    bool sslEnabled = false;
#endif
};

QT_END_NAMESPACE

#endif // QABSTRACTHTTPSERVER_P_H
