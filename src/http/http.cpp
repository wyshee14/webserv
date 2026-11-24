#include "http.hpp"

const std::string& http::getMethod()
{
    return method;
}

const std::string& http::getPath()
{
    return path;
}

const std::string& http::getArg()
{
    return arg;
}

const std::string& http::getVersion()
{
    return version;
}

std::string http::getHeader(const std::string& name)
{
    std::map<std::string, std::string>::const_iterator it =
        header.find(toLowerAsciiEngine(name));

    if (it == header.end())
        return "";

    return it->second;
}

const std::map<std::string, std::string>& http::getHeaders()
{
    return header;
}

const std::string& http::getBody()
{
    return body;
}

http::http()
{
    method = "";
    path = "";
    arg = "";
    version = "";
    body = "";
    contentLength = 0;
    hasContentLength = 0;
}

int http::mustCloseByHttpRules()
{
    std::map<std::string, std::string>::const_iterator it =
      header.find("connection");

    if (it != header.end())
    {
        const std::string connectionValue = toLowerAsciiEngine(it->second);

        if (connectionValue == "close")
            return 1;

        if (connectionValue == "keep-alive")
            return 0;
    }

    return 0;
}
