#include <cpp_restapi/sse_parser.hpp>

#include <charconv>


namespace cpp_restapi
{

std::vector<SseEvent> SseParser::feed(const std::string& chunk)
{
    // Normalize line endings: CRLF and standalone CR become LF
    for (std::size_t i = 0; i < chunk.size(); ++i)
    {
        if (chunk[i] == '\r')
        {
            if (i + 1 < chunk.size() && chunk[i + 1] == '\n')
                ++i;

            m_buffer.push_back('\n');
        }
        else
        {
            m_buffer.push_back(chunk[i]);
        }
    }

    std::vector<SseEvent> events;
    const std::string delimiter = "\n\n";

    std::string::size_type pos = 0;
    while ((pos = m_buffer.find(delimiter)) != std::string::npos)
    {
        std::string block = m_buffer.substr(0, pos);
        m_buffer.erase(0, pos + delimiter.size());

        // skip empty blocks (e.g. from comment-only messages)
        if (block.empty())
            continue;

        events.push_back(parseBlock(block));
    }

    return events;
}


SseEvent SseParser::parseBlock(const std::string& block)
{
    SseEvent event;
    std::string dataAccumulator;

    std::string::size_type lineStart = 0;
    while (lineStart < block.size())
    {
        auto lineEnd = block.find('\n', lineStart);
        if (lineEnd == std::string::npos)
            lineEnd = block.size();

        const std::string line = block.substr(lineStart, lineEnd - lineStart);
        lineStart = lineEnd + 1;

        // lines starting with ':' are comments — ignore
        if (!line.empty() && line[0] == ':')
            continue;

        const auto colonPos = line.find(':');

        std::string field;
        std::string value;

        if (colonPos != std::string::npos)
        {
            field = line.substr(0, colonPos);
            // per spec: if there's a space after the colon, skip it
            const auto valueStart = (colonPos + 1 < line.size() && line[colonPos + 1] == ' ')
                ? colonPos + 2
                : colonPos + 1;
            value = line.substr(valueStart);
        }
        else
        {
            field = line;
        }

        if (field == "data")
        {
            if (!dataAccumulator.empty())
                dataAccumulator += '\n';
            dataAccumulator += value;
        }
        else if (field == "event")
        {
            event.event = value;
        }
        else if (field == "id")
        {
            event.id = value;
        }
        else if (field == "retry")
        {
            int parsed = 0;
            const auto [ptr, ec] = std::from_chars(value.data(), value.data() + value.size(), parsed);
            if (ec == std::errc{} && ptr == value.data() + value.size())
                event.retry = parsed;
        }
    }

    event.data = dataAccumulator;

    return event;
}

}
