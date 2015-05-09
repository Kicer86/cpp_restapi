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

#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "aconnection.hpp"

#include <QNetworkRequest>

class QNetworkAccessManager;
class QString;

namespace GitHub
{
    class Connection: public AConnection
    {
        public:
            Connection(QNetworkAccessManager *, const QString& address, const QString& token);
            Connection(const Connection &) = delete;

            ~Connection();

            Connection& operator=(const Connection &) = delete;

        private:
            QNetworkAccessManager* m_networkManager;
            const QString m_address;
            const QString m_token;

            QNetworkRequest prepareRequest();
    };
}

#endif // CONNECTION_HPP
