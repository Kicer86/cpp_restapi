/*
 * Class representing GitHub Api connection
 * Copyright (C) 2015  Michał Walenciak <MichalWalenciak@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "connection.hpp"

#include <cassert>
#include <string>

#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>


namespace GitHub
{

    Connection::Connection(QNetworkAccessManager* manager, const QString& address, const QString& token):
        m_networkManager(manager),
        m_address(address),
        m_token(token),
        m_reply(nullptr)
    {

    }


    Connection::~Connection()
    {

    }


    QNetworkRequest Connection::prepareRequest()
    {
        QNetworkRequest requst;

        if (m_token.isEmpty() == false)
        {
            const QByteArray key("Authorization");
            const QByteArray value = QString("token %1").arg(m_token).toLatin1();
            requst.setRawHeader(key, value);
        }

        return requst;
    }


    void Connection::get(const QString& query)
    {
        QNetworkRequest request = prepareRequest();
        const QUrl url = QString("%1/%2").arg(m_address).arg(query);
        request.setUrl(url);

        assert(m_reply == nullptr);
        m_reply = m_networkManager->get(request);

        connect(m_reply, SIGNAL(readChannelFinished()), this, SLOT(gotReply()));
    }


    void Connection::gotReply()
    {
        assert(m_reply != nullptr);

        const QByteArray data = m_reply->readAll();
        const QJsonDocument doc = QJsonDocument::fromJson(data);

        emit gotReply(doc);

        m_reply->deleteLater();
        m_reply = nullptr;
    }

}
