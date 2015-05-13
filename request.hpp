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

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <memory>

#include <QObject>
#include <QJsonDocument>

class QEventLoop;

namespace GitHub
{

    struct IConnection;

    class Request
    {
        public:
            Request(IConnection *);
            Request(const Request &) = delete;
            ~Request();

            Request& operator=(const Request &) = delete;

            const QJsonDocument& getUserInfo(const QString& user);
            const QJsonDocument& getReleases(const QString& owner, const QString& repo);
            const QJsonDocument& getRelease(const QString& owner, const QString& repo, int id);

        private:
            IConnection* m_connection;
            QEventLoop* m_eventLoop;
            QJsonDocument m_result;

            const QJsonDocument& doRequest(const QString &);
            void waitForReply();
            void gotReply(const QJsonDocument &);
    };

}

#endif // REQUEST_H
