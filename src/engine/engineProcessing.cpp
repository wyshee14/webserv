#include "engine.hpp"
#include "fileHandler.hpp"

std::string phraseForStatusEngine(int status)
{
    if (status == 200)
        return "OK";
    else if (status == 201)
        return "Created";
    else if (status == 204)
        return "No Content";
    else if (status == 301)
        return "Moved Permanently";
    else if (status == 302)
        return "Found";
    else if (status == 303)
        return "See Other";
    else if (status == 307)
        return "Temporary Redirect";
    else if (status == 308)
        return "Permanent Redirect";
    else if (status == 400)
        return "Bad Request";
    else if (status == 403)
        return "Forbidden";
    else if (status == 404)
        return "Not Found";
    else if (status == 405)
        return "Method Not Allowed";
    else if (status == 408)
        return "Request Timeout";
    else if (status == 413)
        return "Payload Too Large";
    else if (status == 500)
        return "Internal Server Error";
    else if (status == 504)
        return "Gateway Timeout";
    else
        return "Internal Server Error";
}

std::string Engine::buildStandardResponse(int status, const std::string& resp, int c)
{
    std::stringstream str;
    str << status << " " << phraseForStatusEngine(status);

    return buildResponse(str.str(), resp, c,std::vector<std::string>());
}

std::string Engine::buildErrorResponse(int status,int c,const RequestConfig& config)
{
    return buildErrorResponse(status,c,config,std::vector<std::string>());
}

std::string Engine::buildRedirectResponse(int status, const std::string& redirectTarget, int c)
{
    if (status != 301 && status != 302 && status != 303
        && status != 307 && status != 308)
        status = 302;

    std::vector<std::string> headers;
    headers.push_back("Location: " + redirectTarget);

    std::stringstream str;
    str << status << " " << phraseForStatusEngine(status);

    return buildResponse(str.str(),"",c,headers);
}

std::string Engine::buildErrorResponse(int status,int c,const ServerConfig& config)
{
    const std::string phrase = phraseForStatusEngine(status);

    const std::map<int, std::string>& errorPages = config.getErrorPage();

    std::map<int, std::string>::const_iterator errorPage = errorPages.find(status);

    if (errorPage != errorPages.end())
    {
        std::string path =
            config.getRoot() + errorPage->second;

        fileHandler handler;

        if (handler.fileExists(path))
        {
            std::string content = handler.readFile(path);

            std::stringstream str;
            str << status << " " << phrase;

            return buildResponse(str.str(), content, c, std::vector<std::string>());
        }
    }

    std::stringstream str;
    str << status << " " << phrase;

    return buildResponse(str.str(), phrase, c, std::vector<std::string>());
}

std::string Engine::buildResponse(const std::string& status, const std::string& resp, int c, const std::vector<std::string>& s)
{
    std::stringstream str;

    str << "HTTP/1.1 " << status << "\r\n";
    str << "Content-Length: " << resp.size() << "\r\n";

    for (size_t i = 0; i < s.size(); ++i)
        str << s[i] << "\r\n";

    if (c)
        str << "Connection: close\r\n";
    else
        str << "Connection: keep-alive\r\n";

    str << "\r\n";
    str << resp;

    return str.str();
}

std::string Engine::buildErrorResponse(int status,int c,const RequestConfig& config,const std::vector<std::string>& s)
{
    const std::string phrase = phraseForStatusEngine(status);

	const std::map<int, std::string>& errorPages = config.getErrorPage();

	std::map<int, std::string>::const_iterator errorPage = errorPages.find(status);

	if (errorPage != errorPages.end())
	{
		std::string path = config.getRoot() + errorPage->second;

		fileHandler handler;

		if (handler.fileExists(path))
		{
			std::string page = handler.readFile(path);

			std::stringstream str;
			str << status << " " << phrase;

			return buildResponse(str.str(),page,c,s);
		}
	}

    std::stringstream str;
    str << status << " " << phrase;

    return buildResponse(str.str(),phrase,c,s);
}