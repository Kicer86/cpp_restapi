
#include <iostream>

#include <github_api/github_api_curl.hpp>
#include <github_api/request.hpp>


int main(int argc, char** argv)
{
    GitHub::CurlBackend::Api github;
    GitHub::Request request(github.connect());

    std::cout << request.getRateLimit() << '\n';
    std::cout << request.getUserInfo("Kicer86") << '\n';

    return 0;
}
