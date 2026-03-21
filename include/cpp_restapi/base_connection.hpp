
#ifndef BASE_CONNECTION_HPP_INCLUDED
#define BASE_CONNECTION_HPP_INCLUDED

#include <expected>
#include <map>

#include <cpp_restapi/iconnection.hpp>
#include "cpp_restapi_export.h"


namespace cpp_restapi
{
    /**
     * @brief base class with common parts for backend specific implementations
     */
    class CPP_RESTAPI_EXPORT BaseConnection: public cpp_restapi::IConnection
    {
    public:
        explicit BaseConnection(const std::string& address, const std::map<std::string, std::string>& headerEntries);

        std::string get(const std::string &) final;
        std::expected<std::string, HttpError> fetch(const std::string& request) final;
        std::expected<std::string, HttpError> fetch(const std::string& request, IPaginationStrategy& strategy) final;
        std::expected<Response, HttpError> fetchResponse(const std::string& url) final;
        const std::string& url() const final;
        CancellationToken fetch(const std::string& request, FetchCallback onSuccess, ErrorCallback onError = {}) final;
        virtual Response fetchPage(const std::string& request) = 0;

    protected:
        virtual void fetchAsync(const std::string& fullUrl,
                                CancellationToken cancel,
                                FetchCallback onSuccess,
                                ErrorCallback onError) = 0;

        const std::map<std::string, std::string>& getHeaderEntries() const;

        const std::string& address() const;

    private:
        const std::string m_address;
        std::map<std::string, std::string> m_headerEntries;
    };
}

#endif
