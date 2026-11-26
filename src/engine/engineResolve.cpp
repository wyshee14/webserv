
#include "engine.hpp"
#include "http.hpp"
#include "stringUtils.hpp"
#include "fileHandler.hpp"
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <cctype>
#include <vector>
#include <string>

int Engine::checkPath(const std::string& path)
{
    std::string tmp = path;

    for (size_t i = 0; i < tmp.size(); ++i)
        tmp[i] = std::tolower(static_cast<unsigned char>(tmp[i]));

    if (tmp.find("..") != std::string::npos)
        return 1;

    return 0;
}

int Engine::isMethodAllowed(const std::string& method, const RequestConfig& route)
{
    if (!route.hasLocation())
        return 1;

    const std::vector<std::string>& methods = route.getAllowedMethods();

    for (size_t i = 0; i < methods.size(); ++i)
    {
        if (methods[i] == method)
            return 1;
    }

    return 0;
}

std::vector<std::string> Engine::methodNotAllowedHeaders(const RequestConfig& route)
{
    std::string value = "GET, POST, DELETE";

	const std::vector<std::string>& methods = route.getAllowedMethods();

	if (route.hasLocation())
	{
		value.clear();

		for (size_t i = 0; i < methods.size(); ++i)
		{
			if (i > 0)
				value += ", ";

			value += methods[i];
		}
	}

    std::vector<std::string> headers;
    headers.push_back("Allow: " + value);

    return headers;
}

int ensureDirectoryExistsEngine(const std::string& dir)
{
    if (dir.empty())
        return 0;

    struct stat stat;

    if (stat(dir.c_str(), &stat) != 0)
        return 0;

    return S_ISDIR(stat.st_mode) ? 1 : 0;
}

std::string Engine::routeRequest(http& input, int close, const RequestConfig& route)
{
    if (route.getRedirectTarget().empty())
    {
        return buildRedirectResponse(route.getRedirectCode(), route.getRedirectTarget(), close);
    }

    if (input.getMethod() == "GET")
        return processGet(input, close, config);

    if (input.getMethod() == "POST")
        return processPost(input, close, config);

    if (input.getMethod() == "DELETE")
        return processDelete(input, close, config);

    return buildErrorResponse(405, close, &config, methodNotAllowedHeaders(route));
}

std::string Engine::processGet(http& input, int close, const RequestConfig& config)
{
    if (checkPath(input.getPath()))
        return buildErrorResponse(403, close, &config);

	std::string file;

	if (!config.getIndex().empty())
		file = config.getIndex()[0];
		
    fileHandler handler;

    std::string path = handler.resolvePath(input.getPath(), config.getRoot());

    struct stat stat;

    if (stat(path.c_str(), &stat) == 0 && S_ISDIR(stat.st_mode))
    {
        if (!file.empty())
        {
            std::string indexPath = path + "/" + file;

            if (handler.fileExists(indexPath))
                path = indexPath;
            else if (config.getAutoindex())
            {
                std::string content = handler.generateDirectoryListing(input.getPath(),path);

                if (content.empty())
                    return buildErrorResponse(500, close, &config);

                return buildStandardResponse(200,content, close);
            }
            else
                return buildErrorResponse(403, close, &config);
        }
        else if (config.getAutoindex())
        {
            std::string content = handler.generateDirectoryListing(input.getPath(),path);

            if (content.empty())
                return buildErrorResponse(500, close, &config);

            return buildStandardResponse(200,content,close);
        }
        else
            return buildErrorResponse(403, close, &config);
    }

    if (!handler.fileExists(path))
        return buildErrorResponse(404, close, &config);

	std::string content = handler.readFile(path);

	return buildStandardResponse(200,content,close);
}

std::string Engine::processPost(http& input,int close,const RequestConfig& config)
{
	if (config.getUploadDirectory().empty())
		return buildErrorResponse(403, close, config);

    const std::string& body = input.getBody();
    std::string dir = config.getUploadDirectory();

    if (!ensureDirectoryExistsEngine(dir))
        return buildErrorResponse(500, close, &config);

    std::string name = "upload.txt";
    
	if (name.empty())
		return buildErrorResponse(400, close, &config);

    std::string path = dir + "/" + name;

    int fd = open(path.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);

    if (fd < 0)
        return buildErrorResponse(500, close, &config);

    size_t n = 0;

    while (n < body.size())
    {
        ssize_t result = write(fd,body.data() + n,body.size() - n);

        if (result <= 0)
        {
            close(fd);
            return buildErrorResponse(500, close, &config);
        }

        n += static_cast<size_t>(result);
    }

    close(fd);

    return buildStandardResponse(201,"Upload OK",close);
}

std::string Engine::processDelete(http& input,int close,const RequestConfig& config)
{
    if (checkPath(input.getPath()))
        return buildErrorResponse(403, close, &config);
    
	fileHandler handler;

    std::string path = handler.resolvePath(input.getPath(), config.getRoot());

    if (!handler.fileExists(path))
        return buildErrorResponse(404, close, &config);

    if (std::remove(path.c_str()) != 0)
        return buildErrorResponse(500, close, &config);

    return buildStandardResponse(204, "", close);
}
