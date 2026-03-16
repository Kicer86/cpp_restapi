
#include <gmock/gmock.h>

#include <cpp_restapi/link_header_pagination_strategy.hpp>

using namespace cpp_restapi;
using testing::IsEmpty;


TEST(LinkHeaderPaginationStrategyTest, returnsEmptyWhenNoLinkHeader)
{
    LinkHeaderPaginationStrategy strategy;
    EXPECT_THAT(strategy.nextPageUrl("Content-Type: application/json\r\n"), IsEmpty());
}

TEST(LinkHeaderPaginationStrategyTest, returnsEmptyForEmptyHeaders)
{
    LinkHeaderPaginationStrategy strategy;
    EXPECT_THAT(strategy.nextPageUrl(""), IsEmpty());
}

TEST(LinkHeaderPaginationStrategyTest, extractsNextPageUrl)
{
    LinkHeaderPaginationStrategy strategy;
    const auto url = strategy.nextPageUrl(R"(Link: <https://api.example.com/items?page=2>; rel="next")");
    EXPECT_EQ(url, "https://api.example.com/items?page=2");
}

TEST(LinkHeaderPaginationStrategyTest, extractsNextPageFromMultipleRels)
{
    LinkHeaderPaginationStrategy strategy;
    const auto url = strategy.nextPageUrl(
        R"(Link: <https://api.example.com/items?page=1>; rel="prev", <https://api.example.com/items?page=3>; rel="next")");
    EXPECT_EQ(url, "https://api.example.com/items?page=3");
}

TEST(LinkHeaderPaginationStrategyTest, returnsEmptyWhenOnlyPrevRel)
{
    LinkHeaderPaginationStrategy strategy;
    EXPECT_THAT(strategy.nextPageUrl(R"(Link: <https://api.example.com/items?page=1>; rel="prev")"), IsEmpty());
}

TEST(LinkHeaderPaginationStrategyTest, mergesSinglePage)
{
    LinkHeaderPaginationStrategy strategy;
    const auto result = strategy.merge({R"({"key":"value"})"});
    EXPECT_EQ(result, "{\"key\":\"value\"}\n");
}

TEST(LinkHeaderPaginationStrategyTest, mergesObjectPages)
{
    LinkHeaderPaginationStrategy strategy;
    const auto result = strategy.merge({
        R"({"a":1})",
        R"({"b":2})",
        R"({"c":3})"
    });
    EXPECT_EQ(result, "{\"a\":1,\"b\":2,\"c\":3}\n");
}

TEST(LinkHeaderPaginationStrategyTest, mergesArrayPages)
{
    LinkHeaderPaginationStrategy strategy;
    const auto result = strategy.merge({
        R"([{"id":1}])",
        R"([{"id":2}])",
        R"([{"id":3}])"
    });
    EXPECT_EQ(result, "[{\"id\":1},{\"id\":2},{\"id\":3}]\n");
}

TEST(LinkHeaderPaginationStrategyTest, deepMergesNestedObjects)
{
    LinkHeaderPaginationStrategy strategy;
    const auto result = strategy.merge({
        R"({"outer":{"inner1":"a"}})",
        R"({"outer":{"inner2":"b"}})"
    });
    EXPECT_EQ(result, "{\"outer\":{\"inner1\":\"a\",\"inner2\":\"b\"}}\n");
}
