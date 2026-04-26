
#include <utility>

#include <httplib.h>

#include <cpp_restapi/create_cpp-httplib_connection.hpp>
#include "cpp-httplib_sse_connection.hpp"
#include "threaded_connection.hpp"


namespace
{
    class CppHttplibConnection final: public cpp_restapi::ThreadedConnection
    {
        public:
            CppHttplibConnection(const std::string& address, const std::map<std::string, std::string>& headerEntries);
            CppHttplibConnection(const CppHttplibConnection &) = delete;

            CppHttplibConnection& operator=(const CppHttplibConnection &) = delete;

            cpp_restapi::Response fetchPage(const std::string& request) override;
            std::unique_ptr<cpp_restapi::ISseConnection> subscribe(const std::string& request, cpp_restapi::IConnection::EventCallback callback) override;
    };


    CppHttplibConnection::CppHttplibConnection(const std::string& address, const std::map<std::string, std::string>& headerEntries)
        : cpp_restapi::ThreadedConnection(address, headerEntries)
    {

    }


    cpp_restapi::Response CppHttplibConnection::fetchPage(const std::string& page)
    {
        auto pos = page.find("//");
        if (pos == std::string::npos)
            pos = 0;
        else
            pos += 2;

        pos = page.find("/", pos);
        if (pos == std::string::npos)
            return {};

        const std::string server = page.substr(0, pos);
        const std::string query = page.substr(pos);

        httplib::Client cli(server);

        const httplib::Headers headers(getHeaderEntries().begin(), getHeaderEntries().end());
        const auto response = cli.Get(query, headers);

        if (!response)
            return {};  // statusCode = 0: network-level failure

        std::string header;
        for(const auto& [key, value]: response->headers)
            header += key + ": " + value + "\n";

        return {response->body, std::move(header), response->status};
    }


    std::unique_ptr<cpp_restapi::ISseConnection> CppHttplibConnection::subscribe(const std::string& request, cpp_restapi::IConnection::EventCallback callback)
    {
        auto sse = std::make_unique<cpp_restapi::CppHttplibBackend::SseConnection>(address(), getHeaderEntries());
        sse->subscribe(request, std::move(callback));
        return sse;
    }
}


namespace cpp_restapi
{
    std::unique_ptr<IConnection> createCppHttplibConnection(
        const std::string& address,
        const std::map<std::string, std::string>& headerEntries)
    {
        return std::make_unique<CppHttplibConnection>(address, headerEntries);
    }
}
