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

#include "github_api.hpp"

GitHubApi::GitHubApi()
{

}


GitHubApi::~GitHubApi()
{

}


github::connection GitHubApi::connect(const std::string&)
{
    github::connection result;

    return result;
}


github::connection GitHubApi::connect(const std::string& token, const std::string&)
{
    github::connection result;

    return result;
}


constexpr const char* GitHubApi::default_addr()
{
    return "https://api.github.com";
}

