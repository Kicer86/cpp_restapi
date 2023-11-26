
#include <QNetworkAccessManager>
#include <gmock/gmock.h>
#include <httpmockserver/mock_server.h>
#include <httpmockserver/test_environment.h>

#include "cpp_restapi/curl_connection.hpp"
#include "cpp_restapi/qt_connection.hpp"
#include "cpp_restapi/github/github_api_curl.hpp"
#include "cpp_restapi/github/github_api_qt.hpp"
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
    T buildApi();

    template<typename T>
    std::shared_ptr<IConnection> buildNewApi();

    template<>
    GitHub::CurlBackend::Api buildApi<GitHub::CurlBackend::Api>()
    {
        return GitHub::CurlBackend::Api(std::string("http://localhost:") + std::to_string(port));
    }

    template<>
    GitHub::QtBackend::Api buildApi<GitHub::QtBackend::Api>()
    {
        static QNetworkAccessManager networkmanager;
        return GitHub::QtBackend::Api(networkmanager, QString("http://localhost:%1").arg(port));
    }

    template<>
    std::shared_ptr<IConnection> buildNewApi<GitHub::CurlBackend::Api>()
    {
        return GitHub::ConnectionBuilder().setAddress(std::string("http://localhost:") + std::to_string(port)).build<CurlBackend::Connection>();
    }

    template<>
    std::shared_ptr<IConnection>buildNewApi<GitHub::QtBackend::Api>()
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
            server.start();
        }

    protected:
        NiceMock<GithubServerMock> server;
};

template<typename>
struct BackendTraits;

template<>
struct BackendTraits<GitHub::CurlBackend::Api>
{
    using Connection = CurlBackend::Connection;
};

template<>
struct BackendTraits<GitHub::QtBackend::Api>
{
    using Connection = QtBackend::Connection;
};

using Backends = testing::Types<GitHub::CurlBackend::Api, GitHub::QtBackend::Api>;
TYPED_TEST_SUITE(ApiTest, Backends);


TYPED_TEST(ApiTest, fetchRegularUser)
{
    GithubServerMock::Response response(200, R"({"login":"userName1234","id":1234"})");
    ON_CALL(this->server, responseHandler).WillByDefault(Return(response));

    auto api = buildApi<TypeParam>();
    auto connection = api.connect();

    GitHub::Request request(std::move(connection));
    const auto info = request.getUserInfo("userName1234");

    EXPECT_EQ(info, "{\"id\":1234,\"login\":\"userName1234\"}\n");
}


TYPED_TEST(ApiTest, newInterface)
{
    GithubServerMock::Response response(200, R"({"login":"userName1234","id":1234"})");
    ON_CALL(this->server, responseHandler).WillByDefault(Return(response));

    using Connection = typename BackendTraits<TypeParam>::Connection;
    auto connection = buildNewApi<TypeParam>();

    GitHub::Request request(connection);
    const auto info = request.getUserInfo("userName1234");

    EXPECT_EQ(info, "{\"id\":1234,\"login\":\"userName1234\"}\n");
}


TYPED_TEST(ApiTest, pagination)
{
    auto api = buildApi<TypeParam>();
    auto connection = api.connect();

    GithubServerMock::Response response1(200, R"({"login":"userName1234","id":1234})");
    const std::string secondPage = api.address() + "/url/to/second/page&page=2";
    response1.addHeader( {"link", "<" + secondPage + ">; rel=\"next\""} );

    GithubServerMock::Response response2(200, R"({"someotherfield":"value"})");
    const std::string thirdPage = api.address() + "/url/to/last/page&page=3";
    response2.addHeader( {"Link", "<some_previous_page>; rel=\"prev\", <" + thirdPage + ">; rel=\"next\""} );

    GithubServerMock::Response response3(200, R"({"more_fields":"value234"})");

    ON_CALL(this->server, responseHandler("/users/userName1234", _, _, _, _)).WillByDefault(Return(response1));
    ON_CALL(this->server, responseHandler("/url/to/second/page&page=2", _, _, _, _)).WillByDefault(Return(response2));
    ON_CALL(this->server, responseHandler("/url/to/last/page&page=3", _, _, _, _)).WillByDefault(Return(response3));

    GitHub::Request request(std::move(connection));
    const auto info = request.getUserInfo("userName1234");

    EXPECT_EQ(info, "{\"id\":1234,\"login\":\"userName1234\",\"more_fields\":\"value234\",\"someotherfield\":\"value\"}\n");
}


TYPED_TEST(ApiTest, arraysPagination)
{
    auto api = buildApi<TypeParam>();
    auto connection = api.connect();

    GithubServerMock::Response response1(200, R"([{"login":"userName1234","id":1234}])");
    const std::string secondPage = api.address() + "/url/to/second/page&page=2";
    response1.addHeader( {"Link", "<" + secondPage + ">; rel=\"next\""} );

    GithubServerMock::Response response2(200, R"([{"someotherfield":"value"}])");
    const std::string thirdPage = api.address() + "/url/to/last/page&page=3";
    response2.addHeader( {"link", "<" + thirdPage + ">; rel=\"next\""} );

    GithubServerMock::Response response3(200, R"([{"more_fields":"value234"}])");

    ON_CALL(this->server, responseHandler("/users/userName1234", _, _, _, _)).WillByDefault(Return(response1));
    ON_CALL(this->server, responseHandler("/url/to/second/page&page=2", _, _, _, _)).WillByDefault(Return(response2));
    ON_CALL(this->server, responseHandler("/url/to/last/page&page=3", _, _, _, _)).WillByDefault(Return(response3));

    GitHub::Request request(std::move(connection));
    const auto info = request.getUserInfo("userName1234");

    EXPECT_EQ(info, "[{\"id\":1234,\"login\":\"userName1234\"},{\"someotherfield\":\"value\"},{\"more_fields\":\"value234\"}]\n");
}
