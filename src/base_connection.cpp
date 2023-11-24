
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

#include "base_connection.hpp"
#include "header_utils.hpp"


namespace
{
    void update(Json::Value& a, const Json::Value& b)
    {
        if (a.isArray() && b.isArray())
            for (const auto& entry : b)
                a.append(entry);
        else if (a.isObject() && b.isObject())
        {
            for (const auto& key : b.getMemberNames())
                if (a[key].isObject())
                    update(a[key], b[key]);
                else
                    a[key] = b[key];
        }
    }
}

namespace cpp_restapi
{

BaseConnection::BaseConnection(const std::string& address, const std::string& token)
    : m_address(address)
{
    if (token.empty() == false)
        m_headerEntries.emplace("Authorization", "token " + token);
}

BaseConnection::BaseConnection(const std::string& address, const std::map<std::string, std::string>& headerEntries)
    : m_address(address)
    , m_headerEntries(headerEntries)
{

}


std::string BaseConnection::get(const std::string& request)
{
    std::string nextPage = m_address + "/" + request;

    Json::Value output;

    do
    {
        Json::Reader reader;

        const std::pair<std::string, std::string> response = fetchPage(nextPage);
        const std::string& header_links = response.second;

        Json::Value partialOutput;
        reader.parse(response.first, partialOutput);

        if (output.isNull())
            output.swap(partialOutput);
        else
            update(output, partialOutput);

        nextPage = HeaderUtils::getNextPageUrl(header_links);
    }
    while (nextPage.empty() == false);

    Json::FastWriter fastWriter;
    const std::string strOutput = fastWriter.write(output);

    return strOutput;
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
