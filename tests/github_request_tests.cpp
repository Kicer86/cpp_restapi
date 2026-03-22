
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
#include "cpp_restapi/github/connection_builder.hpp"
#include "cpp_restapi/github/request.hpp"

#include "github_server_mock.hpp"


using testing::_;
using testing::Return;
using testing::Contains;

using namespace cpp_restapi;

namespace
{
    constexpr int port = 9010;

    template<typename T>
    std::shared_ptr<IConnection> buildConnection(GitHub::ConnectionBuilder &);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

    template<>
    std::shared_ptr<IConnection> buildConnection<CurlBackend::Connection>(GitHub::ConnectionBuilder& builder)
    {
        return builder.build(createCurlConnection);
    }

    template<>
    std::shared_ptr<IConnection> buildConnection<QtBackend::Connection>(GitHub::ConnectionBuilder& builder)
    {
        static QNetworkAccessManager networkmanager;
        return builder.build(createQtConnection, networkmanager);
    }

    template<>
    std::shared_ptr<IConnection> buildConnection<CppHttplibBackend::Connection>(GitHub::ConnectionBuilder& builder)
    {
       return builder.build(createCppHttplibConnection);
    }

#pragma GCC diagnostic pop

    template<typename T>
    std::shared_ptr<IConnection> buildNewApi(std::function<void(GitHub::ConnectionBuilder &)> c = {})
    {
        auto builder = GitHub::ConnectionBuilder();
        builder.setAddress(std::string("http://127.0.0.1:") + std::to_string(port));

        if (c)
            c(builder);

        return buildConnection<T>(builder);
    }
}

// Case-insensitive comparison function
bool caseInsensitiveEqual(const std::string& a, const std::string& b) {
    return std::equal(a.begin(), a.end(),
                      b.begin(), b.end(),
                      [](char c1, char c2) { return std::tolower(c1) == std::tolower(c2); });
}


// Custom matcher for case-insensitive key comparison
MATCHER_P2(PairCaseInsensitive, expectedKey, expectedValue, "")
{
    return caseInsensitiveEqual(arg.first, expectedKey) && arg.second == expectedValue;
}


template <typename T>
class ApiTest: public testing::Test
{
    public:
        ApiTest(): server(port)
        {

        }

    protected:
        GithubServerMock server;
};


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
using Backends = testing::Types<CurlBackend::Connection, QtBackend::Connection, CppHttplibBackend::Connection>;
#pragma GCC diagnostic pop
TYPED_TEST_SUITE(ApiTest, Backends);


TYPED_TEST(ApiTest, fetchRegularUser)
{
    EXPECT_CALL(this->server, request(_, _)).WillOnce(Return(GithubServerMock::Response{ R"({"login":"userName1234","id":1234"})", {} }));
    this->server.listen();

    auto connection = buildNewApi<TypeParam>();

    GitHub::Request request(connection);
    const auto info = request.getUserInfo("userName1234");

    EXPECT_EQ(info, "{\"id\":1234,\"login\":\"userName1234\"}\n");
}


TYPED_TEST(ApiTest, authorization)
{
    EXPECT_CALL(this->server, request(_, Contains(PairCaseInsensitive("authorization", "token github_token"))))
        .WillOnce(Return(GithubServerMock::Response{ R"({"login":"userName1234","id":1234"})", {} }));

    this->server.listen();

    auto connection = buildNewApi<TypeParam>([](GitHub::ConnectionBuilder& builder)
    {
        builder.setToken("github_token");
    });

    GitHub::Request request(connection);
    const auto info = request.getUserInfo("userName1234");

    EXPECT_EQ(info, "{\"id\":1234,\"login\":\"userName1234\"}\n");
}


TYPED_TEST(ApiTest, requestReturns404ErrorGivesEmptyString)
{
    EXPECT_CALL(this->server, request(_, _))
        .WillOnce(Return(GithubServerMock::Response{"Not Found", {}, 404}));

    this->server.listen();

    auto connection = buildNewApi<TypeParam>([](GitHub::ConnectionBuilder&) {});

    GitHub::Request request(connection);
    const auto result = request.getUserInfo("nonexistent");

    EXPECT_TRUE(result.empty());
}


TYPED_TEST(ApiTest, requestReturns500ErrorGivesEmptyString)
{
    EXPECT_CALL(this->server, request(_, _))
        .WillOnce(Return(GithubServerMock::Response{"Internal Server Error", {}, 500}));

    this->server.listen();

    auto connection = buildNewApi<TypeParam>([](GitHub::ConnectionBuilder&) {});

    GitHub::Request request(connection);
    const auto result = request.getUserInfo("anyuser");

    EXPECT_TRUE(result.empty());
}
