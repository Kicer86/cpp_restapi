#include <cpp_restapi/threaded_connection.hpp>

#include <thread>


namespace cpp_restapi
{

ThreadedConnection::~ThreadedConnection()
{
    waitForPending();
}

void ThreadedConnection::waitForPending()
{
    std::unique_lock lock(m_mutex);
    m_cv.wait(lock, [this]{ return m_activeCount == 0; });
}

void ThreadedConnection::fetchAsync(const std::string& url, CancellationToken cancel, FetchCallback onSuccess, ErrorCallback onError)
{
    {
        std::lock_guard lock(m_mutex);
        ++m_activeCount;
    }

    std::thread([this, url, cancel = std::move(cancel), onSuccess = std::move(onSuccess), onError = std::move(onError)]()
    {
        try
        {
            auto result = fetchResponse(url);

            if (cancel->load(std::memory_order_acquire))
            {
                // cancelled — skip callbacks
            }
            else if (result)
            {
                if (onSuccess)
                    onSuccess(std::move(*result));
            }
            else
            {
                if (onError)
                    onError(std::move(result.error()));
            }
        }
        catch (const std::exception& e)
        {
            if (!cancel->load(std::memory_order_acquire) && onError)
                onError(HttpError{0, {}, e.what()});
        }

        {
            std::lock_guard lock(m_mutex);
            --m_activeCount;
        }
        m_cv.notify_one();
    }).detach();
}

}
