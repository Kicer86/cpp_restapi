
#include <iostream>

#include <github_api/github_api_curl.hpp>
#include <github_api/request.hpp>


int main(int argc, char** argv)
{
    GitHub::CurlBackend::Api github;
    std::unique_ptr<GitHub::IConnection> connection = github.connect();
    GitHub::Request request(connection.get());

    const std::string json = request.getUserInfo("Kicer86");

    std::cout << json.c_str() << '\n';

    return 0;
}
