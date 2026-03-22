
#include <cpp_restapi/create_qt_connection.hpp>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <cpp_restapi/qt_connection.hpp>
#pragma GCC diagnostic pop

namespace cpp_restapi
{
    std::unique_ptr<IConnection> createQtConnection(
        QNetworkAccessManager& manager,
        const std::string& address,
        const std::map<std::string, std::string>& headerEntries)
    {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
        return std::make_unique<QtBackend::Connection>(manager, address, headerEntries);
#pragma GCC diagnostic pop
    }
}
