/*
 * GitHub request
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

#include "request.hpp"
#include "aconnection.hpp"

#include <cassert>
#include <QNetworkReply>
#include <QEventLoop>

namespace GitHub
{

    Request::Request(AConnection* connection): m_connection(connection), m_eventLoop(nullptr), m_result()
    {
    }


    Request::~Request()
    {

    }


    const QJsonDocument& Request::getUserInfo(const QString& user)
    {
        const QString request = QString("users/%1").arg(user);
        return doRequest(request);
    }


    const QJsonDocument& Request::getReleases(const QString& owner, const QString& repo)
    {
        const QString request = QString("repos/%1/%2/releases").arg(owner).arg(repo);
        return doRequest(request);
    }


    const QJsonDocument& Request::getRelease(const QString& owner, const QString& repo, int id)
    {
        const QString request = QString("repos/%1/%2/releases/%3").arg(owner).arg(repo).arg(id);
        return doRequest(request);
    }


    const QJsonDocument& Request::doRequest(const QString& request)
    {
        assert(m_result.isEmpty());

        auto callback = std::bind(&Request::gotReply, this, std::placeholders::_1);

        m_result = QJsonDocument();
        m_connection->get(request, callback);

        waitForReply();

        return m_result;
    }


    void Request::waitForReply()
    {
        assert(m_eventLoop == nullptr);

        m_eventLoop = new QEventLoop;
        m_eventLoop->exec();
    }


    void Request::gotReply(const QJsonDocument& json)
    {
        m_result = json;
        m_eventLoop->exit();
    }

}
