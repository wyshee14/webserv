
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

int Engine::checkPath(const std::string& requestPath)
{
    std::string normalizedPath = requestPath;

    for (size_t i = 0; i < normalizedPath.size(); ++i)
        normalizedPath[i] = std::tolower(static_cast<unsigned char>(normalizedPath[i]));

    if (normalizedPath.find("..") != std::string::npos)
        return 1;

    return 0;
}

std::string Engine::resolveLocationRoot(const ServerConfig& config,const LocationConfig* route)
{
    if (route != NULL && !route->getRoot().empty())
        return route->getRoot();

    return config.getRoot();
}

int Engine::isMethodAllowed(const std::string& requestMethod, const LocationConfig* route)
{
    if (route == NULL)
        return 1;

    const std::vector<std::string>& allowedMethods =
        route->getAllowedMethods();

    if (allowedMethods.empty())
        return 0;

    for (size_t i = 0; i < allowedMethods.size(); ++i)
    {
        if (allowedMethods[i] == requestMethod)
            return 1;
    }

    return 0;
}

std::vector<std::string> Engine::methodNotAllowedHeaders(const LocationConfig* route)
{
    std::string allowedValue = "GET, POST, DELETE";

    if (route != NULL)
    {
        const std::vector<std::string>& allowedMethods =
            route->getAllowedMethods();

        if (!allowedMethods.empty())
        {
            allowedValue.clear();

            for (size_t i = 0; i < allowedMethods.size(); ++i)
            {
                if (i > 0)
                    allowedValue += ", ";

                allowedValue += allowedMethods[i];
            }
        }
    }

    std::vector<std::string> responseHeaders;
    responseHeaders.push_back("Allow: " + allowedValue);

    return responseHeaders;
}

int isUnsafeUploadFilenameEngine(const std::string& filename)
{
    if (filename.empty())
        return 1;

    if (filename == "." || filename == "..")
        return 1;

    if (filename.find('/') != std::string::npos
        || filename.find('\\') != std::string::npos)
        return 1;

    return 0;
}

int ensureDirectoryExistsEngine(const std::string& directoryPath)
{
    if (directoryPath.empty())
        return 0;

    struct stat fileStatus;

    if (stat(directoryPath.c_str(), &fileStatus) != 0)
        return 0;

    return S_ISDIR(fileStatus.st_mode) ? 1 : 0;
}

std::string Engine::mapRequestPathForLocationRoot(const std::string& requestPath, const LocationConfig* route)
{
    if (route == NULL || route->getRoot().empty())
        return requestPath;

    const std::string& configuredPath = route->getPath();

    if (configuredPath.empty() || configuredPath == "/")
        return requestPath;

    if (requestPath == configuredPath)
        return "/";

    if (requestPath.size() > configuredPath.size()
        && requestPath.compare(0, configuredPath.size(), configuredPath) == 0
        && requestPath[configuredPath.size()] == '/')
    {
        return requestPath.substr(configuredPath.size());
    }

    return requestPath;
}

std::string resolveIndexForRequestEngine(const ServerConfig& config, const LocationConfig* route)
{
    const std::vector<std::string>* indexes = NULL;

    if (route != NULL && !route->getIndex().empty())
        indexes = &route->getIndex();
    else
        indexes = &config.getIndex();

    if (indexes->empty())
        return "";

    return (*indexes)[0];
}

std::string Engine::routeRequest(http& input, int close, const ServerConfig& config, const LocationConfig* route)
{
    if (route != NULL && !route->getRedirectTarget().empty())
    {
        return buildRedirectResponse(route->getRedirectCode(), route->getRedirectTarget(), close);
    }

    if (input.getMethod() == "GET")
        return handleGet(input, close, config, route);

    if (input.getMethod() == "POST")
        return handlePost(input, close, config, route);

    if (input.getMethod() == "DELETE")
        return handleDelete(input, close, config, route);

    return buildErrorResponse(405, close, &config, methodNotAllowedHeaders(route));
}

std::string Engine::handleGet(http& input, int close, const ServerConfig& config, const LocationConfig* route)
{
    if (checkPath(input.getPath()))
        return buildErrorResponse(403, close, &config);

    std::string root = resolveLocationRoot(config, route);

    std::string mappedPath = mapRequestPathForLocationRoot(input.getPath(), route);

    fileHandler handler;

    std::string filesystemPath = handler.resolvePath(mappedPath,root);

    struct stat fileStatus;

    if (stat(filesystemPath.c_str(), &fileStatus) == 0 && S_ISDIR(fileStatus.st_mode))
    {
        if (!indexFilename.empty())
        {
            std::string indexPath = filesystemPath + "/" + indexFilename;

            if (handler.fileExists(indexPath))
                filesystemPath = indexPath;
            else if (route != NULL && route->getAutoindex())
            {
                std::string directoryContent =
                  handler.generateDirectoryListing(input.getPath(),filesystemPath);

                if (directoryContent.empty())
                    return buildErrorResponse(500, close, &config);

                return buildStandardResponse(200,directoryContent, close);
            }
            else
                return buildErrorResponse(403, close, &config);
        }
        else if (route != NULL && route->getAutoindex())
        {
            std::string directoryContent = handler.generateDirectoryListing(input.getPath(),filesystemPath);

            if (directoryContent.empty())
                return buildErrorResponse(500, close, &config);

            return buildStandardResponse(200,directoryContent,close);
        }
        else
            return buildErrorResponse(403, close, &config);
    }

    if (!handler.fileExists(filesystemPath))
        return buildErrorResponse(404, close, &config);

	std::string fileContent = handler.readFile(filesystemPath);

	return buildStandardResponse(200,fileContent,close);
}

std::string Engine::handlePost(http& input,int close,const ServerConfig& config,const LocationConfig* route)
{
    if (route == NULL || route->getUploadDirectory().empty())
        return buildErrorResponse(403, close, &config);

    const std::string& requestBody = input.getBody();
    std::string uploadDirectory = route->getUploadDirectory();

    if (!ensureDirectoryExistsEngine(uploadDirectory))
        return buildErrorResponse(500, close, &config);

    std::string uploadFilename = "upload.txt"
    
	if (uploadFilename.empty())
		return buildErrorResponse(400, close, &config);

    std::string uploadFilePath = uploadDirectory + "/" + uploadFilename;

    int fd = open(uploadFilePath.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);

    if (fd < 0)
        return buildErrorResponse(500, close, &config);

    size_t n = 0;

    while (n < requestBody.size())
    {
        ssize_t result = write(fd,requestBody.data() + n,requestBody.size() - n);

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

std::string Engine::handleDelete(http& input,int close,const ServerConfig& config,const LocationConfig* route)
{
    if (checkPath(input.getPath()))
        return buildErrorResponse(403, close, &config);

    std::string root = resolveLocationRoot(config, route);

    std::string mappedPath = mapRequestPathForLocationRoot(input.getPath(), route);

    fileHandler handler;

    std::string filesystemPath = handler.resolvePath(mappedPath, root);

    if (!handler.fileExists(filesystemPath))
        return buildErrorResponse(404, close, &config);

    if (std::remove(filesystemPath.c_str()) != 0)
        return buildErrorResponse(500, close, &config);

    return buildStandardResponse(204, "", close);
}
