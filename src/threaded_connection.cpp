#include <cpp_restapi/threaded_connection.hpp>

#include <thread>


namespace cpp_restapi
{

void ThreadedConnection::fetch(const std::string& url, FetchCallback onSuccess, ErrorCallback onError)
{
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
    }).detach();
}

}
