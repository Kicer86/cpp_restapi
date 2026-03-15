#include <gmock/gmock.h>

#include "cpp_restapi/sse_parser.hpp"

using namespace cpp_restapi;
using testing::IsEmpty;
using testing::SizeIs;


TEST(SseParserTest, emptyInputProducesNoEvents)
{
    SseParser parser;
    const auto events = parser.feed("");
    EXPECT_THAT(events, IsEmpty());
}


TEST(SseParserTest, incompleteBlockProducesNoEvents)
{
    SseParser parser;
    // no double newline yet
    const auto events = parser.feed("data: hello");
    EXPECT_THAT(events, IsEmpty());
}


TEST(SseParserTest, singleDataEvent)
{
    SseParser parser;
    const auto events = parser.feed("data: hello world\n\n");

    ASSERT_THAT(events, SizeIs(1));
    EXPECT_EQ(events[0].data, "hello world");
    EXPECT_EQ(events[0].event, "");
    EXPECT_EQ(events[0].id, "");
    EXPECT_EQ(events[0].retry, -1);
}


TEST(SseParserTest, multipleEventsInOneChunk)
{
    SseParser parser;
    const auto events = parser.feed("data: first\n\ndata: second\n\n");

    ASSERT_THAT(events, SizeIs(2));
    EXPECT_EQ(events[0].data, "first");
    EXPECT_EQ(events[1].data, "second");
}


TEST(SseParserTest, eventSplitAcrossChunks)
{
    SseParser parser;

    auto events = parser.feed("data: hel");
    EXPECT_THAT(events, IsEmpty());

    events = parser.feed("lo\n\n");
    ASSERT_THAT(events, SizeIs(1));
    EXPECT_EQ(events[0].data, "hello");
}


TEST(SseParserTest, allFieldsParsed)
{
    SseParser parser;
    const auto events = parser.feed(
        "event: update\n"
        "data: {\"status\":\"ok\"}\n"
        "id: 42\n"
        "retry: 5000\n\n"
    );

    ASSERT_THAT(events, SizeIs(1));
    EXPECT_EQ(events[0].event, "update");
    EXPECT_EQ(events[0].data, "{\"status\":\"ok\"}");
    EXPECT_EQ(events[0].id, "42");
    EXPECT_EQ(events[0].retry, 5000);
}


TEST(SseParserTest, multiLineData)
{
    SseParser parser;
    const auto events = parser.feed(
        "data: line one\n"
        "data: line two\n"
        "data: line three\n\n"
    );

    ASSERT_THAT(events, SizeIs(1));
    EXPECT_EQ(events[0].data, "line one\nline two\nline three");
}


TEST(SseParserTest, commentsAreIgnored)
{
    SseParser parser;
    const auto events = parser.feed(
        ": this is a comment\n"
        "data: actual data\n\n"
    );

    ASSERT_THAT(events, SizeIs(1));
    EXPECT_EQ(events[0].data, "actual data");
}


TEST(SseParserTest, dataFieldWithoutSpace)
{
    SseParser parser;
    // per spec: "data:no space" → value is "no space"
    const auto events = parser.feed("data:no space\n\n");

    ASSERT_THAT(events, SizeIs(1));
    EXPECT_EQ(events[0].data, "no space");
}


TEST(SseParserTest, fieldWithNoValue)
{
    SseParser parser;
    const auto events = parser.feed("data\n\n");

    ASSERT_THAT(events, SizeIs(1));
    EXPECT_EQ(events[0].data, "");
}


TEST(SseParserTest, retryWithNonNumericValueIsIgnored)
{
    SseParser parser;
    const auto events = parser.feed(
        "retry: abc\n"
        "data: test\n\n"
    );

    ASSERT_THAT(events, SizeIs(1));
    EXPECT_EQ(events[0].retry, -1);
    EXPECT_EQ(events[0].data, "test");
}


TEST(SseParserTest, unknownFieldsAreIgnored)
{
    SseParser parser;
    const auto events = parser.feed(
        "custom: value\n"
        "data: test\n\n"
    );

    ASSERT_THAT(events, SizeIs(1));
    EXPECT_EQ(events[0].data, "test");
}


TEST(SseParserTest, consecutiveEventsAcrossMultipleFeeds)
{
    SseParser parser;

    auto events = parser.feed("data: first\n\n");
    ASSERT_THAT(events, SizeIs(1));
    EXPECT_EQ(events[0].data, "first");

    events = parser.feed("data: second\n\n");
    ASSERT_THAT(events, SizeIs(1));
    EXPECT_EQ(events[0].data, "second");
}
