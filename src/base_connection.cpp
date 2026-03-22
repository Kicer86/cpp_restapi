
#include <cpp_restapi/base_connection.hpp>
#include <cpp_restapi/link_header_pagination_strategy.hpp>


namespace cpp_restapi
{

BaseConnection::BaseConnection(const std::string& address, const std::map<std::string, std::string>& headerEntries)
    : m_address(address)
    , m_headerEntries(headerEntries)
{

}


// -- connection info --

const std::string& BaseConnection::url() const
{
    return m_address;
}


// -- synchronous fetch --

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


// -- asynchronous fetch --

CancellationToken BaseConnection::fetch(const std::string& request, FetchCallback onSuccess, ErrorCallback onError)
{
    auto cancel = std::make_shared<std::atomic<bool>>(false);
    fetchAsync(m_address + "/" + request, cancel, std::move(onSuccess), std::move(onError));
    return cancel;
}


CancellationToken BaseConnection::fetch(const std::string& request, IPaginationStrategy& strategy,
                                         BodyCallback onSuccess, ErrorCallback onError)
{
    auto cancel = std::make_shared<std::atomic<bool>>(false);
    auto pages  = std::make_shared<std::vector<std::string>>();

    // Shared state for the recursive chain of async fetches.
    // We capture strategy by pointer — caller must keep it alive
    // until the final callback fires (same contract as the sync overload).

    std::string firstUrl = m_address + "/" + request;

    auto fetchNextPage = std::make_shared<std::function<void(const std::string&)>>();

    *fetchNextPage = [this, cancel, pages, &strategy,
                      onSuccess = std::move(onSuccess),
                      onError   = std::move(onError),
                      fetchNextPage](const std::string& nextUrl)
    {
        fetchAsync(nextUrl, cancel,
            [this, cancel, pages, &strategy, onSuccess, onError, fetchNextPage](Response resp)
            {
                if (cancel->load(std::memory_order_acquire))
                {
                    *fetchNextPage = nullptr;
                    return;
                }

                std::string next = strategy.nextPageUrl(resp.headers);
                pages->push_back(std::move(resp.body));

                if (next.empty())
                {
                    *fetchNextPage = nullptr;
                    if (onSuccess)
                        onSuccess(strategy.merge(*pages));
                }
                else
                {
                    (*fetchNextPage)(next);
                }
            },
            [onError, fetchNextPage](HttpError err)
            {
                *fetchNextPage = nullptr;
                if (onError)
                    onError(std::move(err));
            });
    };

    (*fetchNextPage)(firstUrl);

    return cancel;
}


// -- deprecated --

std::string BaseConnection::get(const std::string& request)
{
    LinkHeaderPaginationStrategy strategy;
    // Deprecated method: throws std::bad_expected_access<HttpError> on error.
    return fetch(request, strategy).value();
}


// -- protected helpers --

const std::map<std::string, std::string>& BaseConnection::getHeaderEntries() const
{
    return m_headerEntries;
}


const std::string & BaseConnection::address() const
{
    return m_address;
}

}
