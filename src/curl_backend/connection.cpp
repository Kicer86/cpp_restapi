#include <iterator>
#include <cassert>
#include <curl/curl.h>

#include "connection.hpp"

GitHub::CurlBackend::Connection::Connection(const std::string& address, const std::string& token)
    : m_address(address)
    , m_token(token)
{
}


GitHub::CurlBackend::Connection::~Connection()
{
}


std::string GitHub::CurlBackend::Connection::get(const std::string& request)
{
    std::string result;

    CURL* curl = curl_easy_init();

    if (curl)
    {
        curl_slist *list = nullptr;
        std::string authorization;

        typedef size_t (*WriteCallback)(char *ptr, size_t size, size_t nmemb, void *userdata);
        WriteCallback write_callback = [](char *ptr, size_t size, size_t nmemb, void* result_raw)
        {
            assert(size == 1);

            std::string& result = *static_cast<std::string*>(result_raw);
            std::copy(ptr, ptr + nmemb, std::back_inserter(result));

            return nmemb;
        };

        const std::string full_addr = m_address + "/" + request;

        curl_easy_setopt(curl, CURLOPT_URL, full_addr.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "github_api/1.0");

        if (m_token.empty() == false)
        {
            authorization = std::string("Authorization: token ") + m_token;
            list = curl_slist_append(list, authorization.c_str());

            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
        }

        curl_easy_perform(curl);

        curl_slist_free_all(list);
        curl_easy_cleanup(curl);
    }

    return result;
}
