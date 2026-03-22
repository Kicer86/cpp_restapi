
#include <future>
#include <type_traits>
#include <QNetworkAccessManager>
#include <gmock/gmock.h>
#include <httplib.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include "cpp_restapi/cpp-httplib_connection.hpp"
#include "cpp_restapi/curl_connection.hpp"
#include "cpp_restapi/qt_connection.hpp"
#pragma GCC diagnostic pop

#include "cpp_restapi/create_cpp-httplib_connection.hpp"
#include "cpp_restapi/create_curl_connection.hpp"
#include "cpp_restapi/create_qt_connection.hpp"
#include "cpp_restapi/isse_connection.hpp"
#include "cpp_restapi/sse_event.hpp"
#include "github_server_mock.hpp"


using testing::_;
using testing::Return;
using testing::NiceMock;

using namespace cpp_restapi;

namespace
{
    constexpr int port = 9010;

    template<typename T>
    std::unique_ptr<IConnection> buildConnection();

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

    template<>
    std::unique_ptr<IConnection> buildConnection<CurlBackend::Connection>()
    {
        return createCurlConnection(std::string("http://127.0.0.1:") + std::to_string(port), {});
    }

    template<>
    std::unique_ptr<IConnection> buildConnection<QtBackend::Connection>()
    {
        static QNetworkAccessManager networkmanager;
        return createQtConnection(networkmanager, std::string("http://127.0.0.1:") + std::to_string(port), {});
    }

    template<>
    std::unique_ptr<IConnection> buildConnection<CppHttplibBackend::Connection>()
    {
        return createCppHttplibConnection(std::string("http://127.0.0.1:") + std::to_string(port), {});
    }

    template<typename T>
    constexpr bool isQtBackend = std::is_same_v<T, QtBackend::Connection>;

#pragma GCC diagnostic pop
}


template <typename T>
class ConnectionTest: public testing::Test
{
    public:
        ConnectionTest(): server(port)
        {

        }

    protected:
        GithubServerMock server;
};


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
using Backends = testing::Types<CurlBackend::Connection, QtBackend::Connection, CppHttplibBackend::Connection>;
#pragma GCC diagnostic pop
TYPED_TEST_SUITE(ConnectionTest, Backends);


TYPED_TEST(ConnectionTest, pagination)
{
    auto connection = buildConnection<TypeParam>();

    const std::string secondPage = connection->url() + "/url/to/second/page&page=2";
    const std::string thirdPage = connection->url() + "/url/to/last/page&page=3";

    EXPECT_CALL(this->server, request("/users/userName1234", _)).WillOnce(Return(GithubServerMock::Response{ R"({"login":"userName1234","id":1234})", { {"Link", "<" + secondPage + ">; rel=\"next\""} } }));
    EXPECT_CALL(this->server, request("/url/to/second/page&page=2", _)).WillOnce(Return(GithubServerMock::Response{ R"({"someotherfield":"value"})", { {"Link", "<some_previous_page>; rel=\"prev\", <" + thirdPage + ">; rel=\"next\""} } }));
    EXPECT_CALL(this->server, request("/url/to/last/page&page=3", _)).WillOnce(Return(GithubServerMock::Response{ R"({"more_fields":"value234"})", {} }));

    this->server.listen();

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    const auto info = connection->get("users/userName1234");
#pragma GCC diagnostic pop
    EXPECT_EQ(info, "{\"id\":1234,\"login\":\"userName1234\",\"more_fields\":\"value234\",\"someotherfield\":\"value\"}\n");
}


TYPED_TEST(ConnectionTest, arraysPagination)
{
    auto connection = buildConnection<TypeParam>();

    const std::string secondPage = connection->url() + "/url/to/second/page&page=2";
    const std::string thirdPage = connection->url() + "/url/to/last/page&page=3";

    EXPECT_CALL(this->server, request("/users/userName1234", _)).WillOnce(Return(GithubServerMock::Response{ R"([{"login":"userName1234","id":1234}])", { {"Link", "<" + secondPage + ">; rel=\"next\""} } }));
    EXPECT_CALL(this->server, request("/url/to/second/page&page=2", _)).WillOnce(Return(GithubServerMock::Response{ R"([{"someotherfield":"value"}])", { {"Link", "<some_previous_page>; rel=\"prev\", <" + thirdPage + ">; rel=\"next\""} } }));
    EXPECT_CALL(this->server, request("/url/to/last/page&page=3", _)).WillOnce(Return(GithubServerMock::Response{ R"([{"more_fields":"value234"}])", {} }));

    this->server.listen();

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    const auto info = connection->get("users/userName1234");
#pragma GCC diagnostic pop
    EXPECT_EQ(info, "[{\"id\":1234,\"login\":\"userName1234\"},{\"someotherfield\":\"value\"},{\"more_fields\":\"value234\"}]\n");
}


TYPED_TEST(ConnectionTest, fetchSuccessReturnsBody)
{
    auto connection = buildConnection<TypeParam>();

    EXPECT_CALL(this->server, request("/api/data", _))
        .WillOnce(Return(GithubServerMock::Response{R"({"key":"value"})", {}, 200}));

    this->server.listen();

    const auto result = connection->fetch("api/data");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), "{\"key\":\"value\"}");
}


TYPED_TEST(ConnectionTest, fetchReturns404Error)
{
    auto connection = buildConnection<TypeParam>();

    EXPECT_CALL(this->server, request("/missing", _))
        .WillOnce(Return(GithubServerMock::Response{"Not Found", {}, 404}));

    this->server.listen();

    const auto result = connection->fetch("missing");
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().statusCode, 404);
}


TYPED_TEST(ConnectionTest, fetchReturns500Error)
{
    auto connection = buildConnection<TypeParam>();

    EXPECT_CALL(this->server, request("/broken", _))
        .WillOnce(Return(GithubServerMock::Response{"Internal Server Error", {}, 500}));

    this->server.listen();

    const auto result = connection->fetch("broken");
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().statusCode, 500);
    EXPECT_EQ(result.error().body, "Internal Server Error");
}


TYPED_TEST(ConnectionTest, asyncFetchCallsOnSuccess)
{
    if constexpr (isQtBackend<TypeParam>)
        GTEST_SKIP() << "Qt backend requires event loop for async operations";

    auto connection = buildConnection<TypeParam>();

    EXPECT_CALL(this->server, request("/async/ok", _))
        .WillOnce(Return(GithubServerMock::Response{R"({"async":"ok"})", {}, 200}));

    this->server.listen();

    std::promise<cpp_restapi::Response> promise;
    auto future = promise.get_future();

    connection->fetch("async/ok",
        [&promise](cpp_restapi::Response resp) { promise.set_value(std::move(resp)); },
        [&promise](cpp_restapi::HttpError err) { promise.set_exception(std::make_exception_ptr(std::runtime_error(err.body))); });

    ASSERT_EQ(future.wait_for(std::chrono::seconds(5)), std::future_status::ready);
    auto resp = future.get();
    EXPECT_EQ(resp.body, R"({"async":"ok"})");
    EXPECT_EQ(resp.statusCode, 200);
}


TYPED_TEST(ConnectionTest, asyncFetchCallsOnError)
{
    if constexpr (isQtBackend<TypeParam>)
        GTEST_SKIP() << "Qt backend requires event loop for async operations";

    auto connection = buildConnection<TypeParam>();

    EXPECT_CALL(this->server, request("/async/fail", _))
        .WillOnce(Return(GithubServerMock::Response{"Server Error", {}, 500}));

    this->server.listen();

    std::promise<cpp_restapi::HttpError> promise;
    auto future = promise.get_future();

    connection->fetch("async/fail",
        [](cpp_restapi::Response) { FAIL() << "onSuccess should not be called"; },
        [&promise](cpp_restapi::HttpError err) { promise.set_value(std::move(err)); });

    ASSERT_EQ(future.wait_for(std::chrono::seconds(5)), std::future_status::ready);
    auto err = future.get();
    EXPECT_EQ(err.statusCode, 500);
}


TYPED_TEST(ConnectionTest, sseSubscribeReceivesEvents)
{
    if constexpr (isQtBackend<TypeParam>)
        GTEST_SKIP() << "Qt backend requires event loop for SSE";

    this->server.setSseEvents({
        {"greeting", "hello world", "1"},
        {"", "plain data", "2"},
        {"update", "something changed", "3"},
    });
    this->server.listen();

    auto connection = buildConnection<TypeParam>();

    std::promise<std::vector<cpp_restapi::SseEvent>> promise;
    auto future = promise.get_future();

    std::vector<cpp_restapi::SseEvent> received;
    auto sseConn = connection->subscribe("sse",
        [&received, &promise](const cpp_restapi::SseEvent& event)
        {
            received.push_back(event);
            if (received.size() == 3)
                promise.set_value(received);
        });

    ASSERT_EQ(future.wait_for(std::chrono::seconds(5)), std::future_status::ready);
    auto events = future.get();

    ASSERT_EQ(events.size(), 3u);
    EXPECT_EQ(events[0].event, "greeting");
    EXPECT_EQ(events[0].data, "hello world");
    EXPECT_EQ(events[0].id, "1");
    EXPECT_EQ(events[1].data, "plain data");
    EXPECT_EQ(events[2].event, "update");
    EXPECT_EQ(events[2].data, "something changed");

    sseConn->close();
}
