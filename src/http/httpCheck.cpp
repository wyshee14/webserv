#include "http.hpp"
#include "stringUtils.hpp"

#include <sstream>
#include <stdexcept>
#include <cerrno>
#include <cstdlib>

int containsChunked(const std::map<std::string, std::string>& headerFields)
{
    std::map<std::string, std::string>::const_iterator header =
        headerFields.find("transfer-encoding");

    if (header == headerFields.end())
        return 0;

    return scanChunkedTransferCodingValue(header->second);
}

int http::getRequest(const std::string& line)
{
    std::stringstream st(line);

    if (!(st >> method >> path >> version))
		return 1;

    size_t pos = path.find('?');

    if (pos != std::string::npos)
    {
        if (pos + 1 < path.size())
            arg = path.substr(pos + 1);

        path = path.substr(0, pos);
    }

    if (path.empty() || path[0] != '/')
        return 1;

    if (version != "HTTP/1.1")
        return 1;

	return 0;
}

int http::getHeaderFields(const std::string& headerSection)
{
    std::stringstream st(headerSection);
    std::string currentLine;

    std::getline(st, currentLine);

    while (std::getline(st, currentLine))
    {
        if (!currentLine.empty()
            && currentLine[currentLine.size() - 1] == '\r')
        {
            currentLine.erase(currentLine.size() - 1);
        }

        if (currentLine.empty())
            continue;

        size_t pos = currentLine.find(':');

        if (pos == std::string::npos)
            return 1;

        std::string headerName =
            toLowerAsciiEngine(currentLine.substr(0, pos));

        std::string headerValue =
            trimAsciiEngine(currentLine.substr(pos + 1));

        if (headerName.empty())
            return 1;

        if (header.find(headerName) != header.end())
            return 1;

        header[headerName] = headerValue;
    }
    return 0;
}

int http::verifyHeader(size_t maximumBodySize)
{
    std::map<std::string, std::string>::const_iterator hostEntry =
        header.find("host");

    if (hostEntry == header.end())
        return 1;

    if (hostEntry->second.empty())
        return 1;

    int hasChunkedPayload = containsChunked(header);

    if (hasChunkedPayload
        && header.find("content-length") != header.end())
    {
        return 1;
    }

    std::map<std::string, std::string>::const_iterator lengthEntry =
        header.find("content-length");

    if (lengthEntry != header.end())
    {
        const std::string& lengthText = lengthEntry->second;

        if (lengthText.empty())
            return 1;

        errno = 0;

        char* convert = NULL;

        unsigned long length =
          std::strtoul(lengthText.c_str(), &convert, 10);

        if (convert == lengthText.c_str()
            || *convert != '\0'
            || errno == ERANGE)
        {
            return 1;
        }

        contentLength = static_cast<size_t>(length);

        if (contentLength > maximumBodySize)
            return 1;

        hasContentLength = 1;
    }
    return 0;
}

void http::init()
{
    method.clear();
    path.clear();
    arg.clear();
    version.clear();
    body.clear();
    header.clear();
    contentLength = 0;
    hasContentLength = 0;
}

int http::extractPayload(const std::string& data, size_t pos,size_t maximumBodySize)
{
    size_t payloadStart = pos + 4;

    if (payloadStart > data.size())
        return 1;

    if (containsChunked(header))
    {
        int oversizedPayload = 0;

        if (!getChunkedPayloadForHttpRequest(data.substr(payloadStart),maximumBodySize,body,&oversizedPayload))
        {
            return 1;
        }

        return 0;
    }

    if (hasContentLength)
    {
        if (payloadStart + contentLength > data.size())
            return 1;

        body = data.substr(payloadStart, contentLength);
    }
    else
    {
        body.clear();
    }
    return 0;
}

int http::process(const std::string& data, size_t maximumBodySize)
{
    init();

    size_t pos = data.find("\r\n\r\n");

    if (pos == std::string::npos)
        return 1;

    size_t linePosition = data.find("\r\n");

    if (linePosition == std::string::npos)
        return 1;

    std::string line = data.substr(0, linePosition);

    if (getRequest(line))
        return 1;

    std::string headerSection = data.substr(0, pos);

    if (getHeaderFields(headerSection))
        return 1;
    if (verifyHeader(maximumBodySize))
        return 1;

    if (extractPayload(data, pos, maximumBodySize))
        return 1;
    return 0;
}
