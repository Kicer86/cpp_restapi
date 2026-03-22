
#ifndef CREATE_QT_CONNECTION_HPP_INCLUDED
#define CREATE_QT_CONNECTION_HPP_INCLUDED

#include <map>
#include <memory>
#include <string>

#include "cpp_restapi_export.h"

class QNetworkAccessManager;

namespace cpp_restapi
{
    struct IConnection;

    /**
     * @brief Create a connection using the Qt backend
     * @param manager Qt network access manager (must outlive the connection)
     * @param address base URL of the API (e.g. "https://api.github.com")
     * @param headerEntries HTTP headers added to every request
     * @return owning pointer to the connection
     */
    CPP_RESTAPI_EXPORT std::unique_ptr<IConnection> createQtConnection(
        QNetworkAccessManager& manager,
        const std::string& address,
        const std::map<std::string, std::string>& headerEntries);
}

#endif
