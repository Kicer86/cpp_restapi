#include <cpp_restapi/threaded_connection.hpp>

#include <thread>


namespace cpp_restapi
{

ThreadedConnection::~ThreadedConnection()
{
    std::unique_lock lock(m_mutex);
    m_cv.wait(lock, [this]{ return m_activeCount == 0; });
}

void ThreadedConnection::fetch(const std::string& url, FetchCallback onSuccess, ErrorCallback onError)
{
    {
        std::lock_guard lock(m_mutex);
        ++m_activeCount;
    }

    std::thread([this, url, onSuccess = std::move(onSuccess), onError = std::move(onError)]()
    {
        try
        {
            auto result = fetchResponse(url);
            if (result)
            {
                if (onSuccess)
                    onSuccess(std::move(*result));
            }
            else
            {
                if (onError)
                    onError(result.error().message);
            }
        }
        catch (const std::exception& e)
        {
            if (onError)
                onError(e.what());
        }

        {
            std::lock_guard lock(m_mutex);
            --m_activeCount;
        }
        m_cv.notify_one();
    }).detach();
}

}
