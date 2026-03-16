#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

#include <cpp_restapi/github/link_header_pagination_strategy.hpp>
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

namespace cpp_restapi::GitHub
{

std::string LinkHeaderPaginationStrategy::nextPageUrl(const std::string& responseHeaders)
{
    return HeaderUtils::getNextPageUrl(responseHeaders);
}


std::string LinkHeaderPaginationStrategy::merge(const std::vector<std::string>& pages)
{
    Json::Value output;
    Json::Reader reader;

    for (const auto& page : pages)
    {
        Json::Value parsed;
        reader.parse(page, parsed);

        if (output.isNull())
            output.swap(parsed);
        else
            update(output, parsed);
    }

    Json::FastWriter fastWriter;
    return fastWriter.write(output);
}

}
