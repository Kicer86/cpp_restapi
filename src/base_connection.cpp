
#include <cpp_restapi/base_connection.hpp>
#include <cpp_restapi/link_header_pagination_strategy.hpp>


namespace cpp_restapi
{

BaseConnection::BaseConnection(const std::string& address, const std::map<std::string, std::string>& headerEntries)
    : m_address(address)
    , m_headerEntries(headerEntries)
{

}


std::string BaseConnection::get(const std::string& request)
{
    LinkHeaderPaginationStrategy strategy;
    return fetch(request, strategy);
}


std::string BaseConnection::fetch(const std::string& request)
{
    return fetchResponse(m_address + "/" + request).body;
}


std::string BaseConnection::fetch(const std::string& request, IPaginationStrategy& strategy)
{
    std::string nextUrl = m_address + "/" + request;
    std::vector<std::string> pages;

    do
    {
        auto resp = fetchResponse(nextUrl);
        pages.push_back(std::move(resp.body));
        nextUrl = strategy.nextPageUrl(resp.headers);
    }
    while (!nextUrl.empty());

    return strategy.merge(pages);
}


const std::string& BaseConnection::url() const
{
    return m_address;
}


Response BaseConnection::fetchResponse(const std::string& requestUrl)
{
    auto [body, headers] = fetchPage(requestUrl);
    return {std::move(body), std::move(headers)};
}


const std::map<std::string, std::string>& BaseConnection::getHeaderEntries() const
{
    return m_headerEntries;
}


const std::string & BaseConnection::address() const
{
    return m_address;
}

}
