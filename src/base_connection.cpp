
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
    // Deprecated method: throws std::bad_expected_access<HttpError> on error.
    return fetch(request, strategy).value();
}


std::expected<std::string, HttpError> BaseConnection::fetch(const std::string& request)
{
    return fetchResponse(m_address + "/" + request)
        .transform([](Response&& resp) { return std::move(resp.body); });
}


std::expected<std::string, HttpError> BaseConnection::fetch(const std::string& request, IPaginationStrategy& strategy)
{
    std::string nextUrl = m_address + "/" + request;
    std::vector<std::string> pages;

    do
    {
        auto result = fetchResponse(nextUrl);
        if (!result)
            return std::unexpected(std::move(result.error()));

        nextUrl = strategy.nextPageUrl(result->headers);
        pages.push_back(std::move(result->body));
    }
    while (!nextUrl.empty());

    return strategy.merge(pages);
}


const std::string& BaseConnection::url() const
{
    return m_address;
}


std::expected<Response, HttpError> BaseConnection::fetchResponse(const std::string& requestUrl)
{
    Response resp = fetchPage(requestUrl);

    if (resp.statusCode == 0)
        return std::unexpected(HttpError{0, std::move(resp.body), "Network error: no response received"});

    if (resp.statusCode >= 400)
        return std::unexpected(HttpError{resp.statusCode, std::move(resp.body),
                                         "HTTP error " + std::to_string(resp.statusCode)});

    return resp;
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
