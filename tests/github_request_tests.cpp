
#include <QNetworkAccessManager>
#include <gmock/gmock.h>
#include <httplib.h>

#include "cpp_restapi/cpp-httplib_connection.hpp"
#include "cpp_restapi/curl_connection.hpp"
#include "cpp_restapi/qt_connection.hpp"
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

    template<>
    std::shared_ptr<IConnection> buildConnection<CurlBackend::Connection>(GitHub::ConnectionBuilder& builder)
    {
        return builder.build<CurlBackend::Connection>();
    }

    template<>
    std::shared_ptr<IConnection> buildConnection<QtBackend::Connection>(GitHub::ConnectionBuilder& builder)
    {
        static QNetworkAccessManager networkmanager;
        return builder.build<QtBackend::Connection>(networkmanager);
    }

    template<>
    std::shared_ptr<IConnection> buildConnection<CppHttplibBackend::Connection>(GitHub::ConnectionBuilder& builder)
    {
       return builder.build<CppHttplibBackend::Connection>();
    }

    template<typename T>
    std::shared_ptr<IConnection> buildNewApi(std::function<void(GitHub::ConnectionBuilder &)> c = {})
    {
        auto builder = GitHub::ConnectionBuilder();
        builder.setAddress(std::string("http://localhost:") + std::to_string(port));

        if (c)
            c(builder);

        return buildConnection<T>(builder);
    }
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


using Backends = testing::Types<CurlBackend::Connection, QtBackend::Connection, CppHttplibBackend::Connection>;
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
    EXPECT_CALL(this->server, request(_, Contains(std::pair<std::string, std::string>{"Authorization", "token github_token"}))).WillOnce(Return(GithubServerMock::Response{ R"({"login":"userName1234","id":1234"})", {} }));
    this->server.listen();

    auto connection = buildNewApi<TypeParam>([](GitHub::ConnectionBuilder& builder)
    {
        builder.setToken("github_token");
    });

    GitHub::Request request(connection);
    const auto info = request.getUserInfo("userName1234");

    EXPECT_EQ(info, "{\"id\":1234,\"login\":\"userName1234\"}\n");
}
