
#include <QNetworkAccessManager>
#include <gmock/gmock.h>
#include <httplib.h>

#include "cpp_restapi/cpp-httplib_connection.hpp"
#include "cpp_restapi/curl_connection.hpp"
#include "cpp_restapi/qt_connection.hpp"

#include "github_server_mock.hpp"


using testing::_;
using testing::Return;
using testing::NiceMock;

using namespace cpp_restapi;

namespace
{
    constexpr int port = 9010;

    template<typename T>
    auto buildConnection();

    template<>
    auto buildConnection<CurlBackend::Connection>()
    {
        return std::make_unique<CurlBackend::Connection>(std::string("http://localhost:") + std::to_string(port), std::map<std::string, std::string>{});
    }

    template<>
    auto buildConnection<QtBackend::Connection>()
    {
        static QNetworkAccessManager networkmanager;
        return std::make_unique<QtBackend::Connection>(networkmanager, std::string("http://localhost:") + std::to_string(port), std::map<std::string, std::string>{});
    }

    template<>
    auto buildConnection<CppHttplibBackend::Connection>()
    {
        return std::make_unique<CppHttplibBackend::Connection>(std::string("http://localhost:") + std::to_string(port), std::map<std::string, std::string>{});
    }
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


using Backends = testing::Types<CurlBackend::Connection, QtBackend::Connection, CppHttplibBackend::Connection>;
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

    const auto info = connection->get("users/userName1234");
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

    const auto info = connection->get("users/userName1234");
    EXPECT_EQ(info, "[{\"id\":1234,\"login\":\"userName1234\"},{\"someotherfield\":\"value\"},{\"more_fields\":\"value234\"}]\n");
}
