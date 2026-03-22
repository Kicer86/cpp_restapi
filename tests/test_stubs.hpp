#ifndef TEST_STUBS_HPP_INCLUDED
#define TEST_STUBS_HPP_INCLUDED

#include <map>
#include <string>
#include <chrono>
#include <thread>

#include <cpp_restapi/isse_connection.hpp>
#include <cpp_restapi/threaded_connection.hpp>

namespace test_stubs
{

    class StubConnection: public cpp_restapi::ThreadedConnection
    {
    public:
        using ThreadedConnection::ThreadedConnection;

        struct PageResponse
        {
            std::string body;
            std::string headers;
            int statusCode = 200;
        };

        void addPage(const std::string& url, PageResponse response)
        {
            m_pages[url] = std::move(response);
        }

        cpp_restapi::Response fetchPage(const std::string& request) override
        {
            auto it = m_pages.find(request);
            if (it != m_pages.end())
                return {it->second.body, it->second.headers, it->second.statusCode};

            return {"" , "", 0};
        }

        std::unique_ptr<cpp_restapi::ISseConnection> subscribe(const std::string&, EventCallback) override
        {
            return nullptr;
        }

    private:
        std::map<std::string, PageResponse> m_pages;
    };


    class ThrowingStubConnection : public cpp_restapi::ThreadedConnection
    {
    public:
        using ThreadedConnection::ThreadedConnection;

        std::atomic<bool> fetchPageCalled{false};

        cpp_restapi::Response fetchPage(const std::string&) override
        {
            fetchPageCalled.store(true, std::memory_order_release);
            throw std::runtime_error("simulated fetch error");
        }

        std::unique_ptr<cpp_restapi::ISseConnection> subscribe(const std::string&, EventCallback) override
        {
            return nullptr;
        }
    };


    class SlowStubConnection : public cpp_restapi::ThreadedConnection
    {
    public:
        using ThreadedConnection::ThreadedConnection;

        ~SlowStubConnection() override { waitForPending(); }

        cpp_restapi::Response fetchPage(const std::string&) override
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            return {"slow body", "", 200};
        }

        std::unique_ptr<cpp_restapi::ISseConnection> subscribe(const std::string&, EventCallback) override
        {
            return nullptr;
        }
    };

}

#endif
