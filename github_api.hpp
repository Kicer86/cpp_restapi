/*
 * GitHub Api
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

#ifndef GITHUBAPI_HPP
#define GITHUBAPI_HPP

// Based on:
// https://developer.github.com/guides/getting-started/
// https://developer.github.com/v3/

#include <memory>

#include <QString>

#include "iconnection.hpp"
#include "github_api_export.h"

class QNetworkAccessManager;

class GITHUB_API_EXPORT GitHubApi
{
    public:
        GitHubApi(const QString& addr = "https://api.github.com");
        GitHubApi(const GitHubApi &) = delete;
        ~GitHubApi();

        void set(QNetworkAccessManager *);

        std::unique_ptr<GitHub::IConnection> connect();                        // anonymous access
        std::unique_ptr<GitHub::IConnection> connect(const QString& token);

        GitHubApi& operator=(const GitHubApi &) = delete;

    private:
        QNetworkAccessManager* m_manager;
        QString m_addres;
};

#endif // GITHUBAPI_HPP

