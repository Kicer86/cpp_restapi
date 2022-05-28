
#include <QNetworkAccessManager>
#include <gmock/gmock.h>
#include <httpmockserver/mock_server.h>
#include <httpmockserver/test_environment.h>

#include "github_api/github_api_curl.hpp"
#include "github_api/github_api_qt.hpp"
#include "github_api/request.hpp"

#include "github_server_mock.hpp"


using testing::Return;
using testing::NiceMock;


namespace
{
    constexpr int port = 9010;

    template<typename T>
    T buildApi();

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

    EXPECT_EQ(info, R"({"login":"userName1234","id":1234"})");
}
