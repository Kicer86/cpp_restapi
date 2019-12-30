/*
 * Photo Broom - photos management tool.
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

#include "github_api.hpp"

#include "connection.hpp"

GitHubApi::GitHubApi(const QString& addr): m_manager(nullptr), m_addres(addr)
{

}


GitHubApi::~GitHubApi()
{

}


void GitHubApi::set(QNetworkAccessManager* manager)
{
    m_manager = manager;
}


std::unique_ptr<GitHub::IConnection> GitHubApi::connect()
{
    std::unique_ptr<GitHub::IConnection> result(new GitHub::Connection(m_manager, m_addres, ""));

    return result;
}


std::unique_ptr<GitHub::IConnection> GitHubApi::connect(const QString& token)
{
    std::unique_ptr<GitHub::IConnection> result(new GitHub::Connection(m_manager, m_addres, token));

    return result;
}


