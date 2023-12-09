
#include <httplib.h>

#include <cpp_restapi/cpp-httplib_connection.hpp>


namespace cpp_restapi::CppHttplibBackend
{
    Connection::Connection(const std::string& address, const std::map<std::string, std::string>& headerEntries)
        : BaseConnection(address, headerEntries)
    {

    }


    std::pair<std::string, std::string> Connection::fetchPage(const std::string& page)
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

        std::string header;
        for(const auto& [key, value]: response->headers)
            header += key + ": " + value + "\n";

        return {response->body, header};
    }
}
