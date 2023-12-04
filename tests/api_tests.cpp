
#include <QNetworkAccessManager>
#include <gmock/gmock.h>
#include <httplib.h>

#include "cpp_restapi/curl_connection.hpp"
#include "cpp_restapi/qt_connection.hpp"
#include "cpp_restapi/github/connection_builder.hpp"
#include "cpp_restapi/github/request.hpp"

#include "github_server_mock.hpp"


using testing::_;
using testing::Return;
using testing::NiceMock;

using namespace cpp_restapi;

namespace
{
    constexpr int port = 9010;

    template<typename T>
    std::shared_ptr<IConnection> buildNewApi();

    template<>
    std::shared_ptr<IConnection> buildNewApi<CurlBackend::Connection>()
    {
        return GitHub::ConnectionBuilder().setAddress(std::string("http://localhost:") + std::to_string(port)).build<CurlBackend::Connection>();
    }

    template<>
    std::shared_ptr<IConnection> buildNewApi<QtBackend::Connection>()
    {
        static QNetworkAccessManager networkmanager;
        return GitHub::ConnectionBuilder().setAddress(std::string("http://localhost:") + std::to_string(port)).build<QtBackend::Connection>(networkmanager);
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


using Backends = testing::Types<CurlBackend::Connection, QtBackend::Connection>;
TYPED_TEST_SUITE(ApiTest, Backends);


TYPED_TEST(ApiTest, fetchRegularUser)
{
    this->server.responseHandler(".*", 200, R"({"login":"userName1234","id":1234"})");
    this->server.listen();

    auto connection = buildNewApi<TypeParam>();

    GitHub::Request request(std::move(connection));
    const auto info = request.getUserInfo("userName1234");

    EXPECT_EQ(info, "{\"id\":1234,\"login\":\"userName1234\"}\n");
}


TYPED_TEST(ApiTest, pagination)
{
    auto connection = buildNewApi<TypeParam>();

    const std::string secondPage = connection->url() + "/url/to/second/page&page=2";
    const std::string thirdPage = connection->url() + "/url/to/last/page&page=3";

    this->server.responseHandler("/users/userName1234",         200, R"({"login":"userName1234","id":1234})",   { {"Link", "<" + secondPage + ">; rel=\"next\""} } );
    this->server.responseHandler("/url/to/second/page&page=2",  200, R"({"someotherfield":"value"}))",          { {"Link", "<some_previous_page>; rel=\"prev\", <" + thirdPage + ">; rel=\"next\""} } );
    this->server.responseHandler("/url/to/last/page&page=3",    200, R"({"more_fields":"value234"})");
    this->server.listen();

    GitHub::Request request(std::move(connection));
    const auto info = request.getUserInfo("userName1234");

    EXPECT_EQ(info, "{\"id\":1234,\"login\":\"userName1234\",\"more_fields\":\"value234\",\"someotherfield\":\"value\"}\n");
}


TYPED_TEST(ApiTest, arraysPagination)
{
    auto connection = buildNewApi<TypeParam>();

    const std::string secondPage = connection->url() + "/url/to/second/page&page=2";
    const std::string thirdPage = connection->url() + "/url/to/last/page&page=3";

    this->server.responseHandler("/users/userName1234",         200, R"([{"login":"userName1234","id":1234}])", { {"Link", "<" + secondPage + ">; rel=\"next\""} } );
    this->server.responseHandler("/url/to/second/page&page=2",  200, R"([{"someotherfield":"value"}]))",        { {"Link", "<some_previous_page>; rel=\"prev\", <" + thirdPage + ">; rel=\"next\""} } );
    this->server.responseHandler("/url/to/last/page&page=3",    200, R"([{"more_fields":"value234"}])");
    this->server.listen();

    GitHub::Request request(std::move(connection));
    const auto info = request.getUserInfo("userName1234");

    EXPECT_EQ(info, "[{\"id\":1234,\"login\":\"userName1234\"},{\"someotherfield\":\"value\"},{\"more_fields\":\"value234\"}]\n");
}
