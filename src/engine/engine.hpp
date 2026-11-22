
#ifndef ENGINE_HPP
# define ENGINE_HPP

# include <string>
# include <vector>
# include <sstream>
# include <sys/types.h>
# include <cstddef>
# include "fileHandler.hpp"
# include "http.hpp"

class Engine
{
	public:
		int checkPath(const std::string& requestPath);
		std::string resolveLocationRoot(const ServerConfig& config, const LocationConfig* route);
		int isMethodAllowed(const std::string& requestMethod, const LocationConfig* route);
		std::vector<std::string> methodNotAllowedHeaders(const LocationConfig* route);
		std::string mapRequestPathForLocationRoot(const std::string& requestPath, const LocationConfig* route);
		std::string handleIncoming(const std::string& req, ServerConfig* cf);
		std::string routeRequest(const http& input, int close, const ServerConfig& config, const LocationConfig* route);
		std::string handleGet(const http& input, int close, const ServerConfig& config, const LocationConfig* route);
		std::string handlePost(const http& input, int close, const ServerConfig& config, const LocationConfig* route);
		std::string handleDelete(const http& input, int close, const ServerConfig& config, const LocationConfig* route);
		std::string buildStandardResponse(int statusCode, const std::string& responseBody, int close);
		std::string buildRedirectResponse(int statusCode, const std::string& redirectTarget, int close);
		std::string buildResponse(const std::string& status, const std::string& responseBody, int close, const std::vector<std::string>& s);
		std::string appendHeaderToResponse(const std::string& response, const std::string& headerLine);
		std::string buildErrorResponse(int statusCode, int close, const ServerConfig* config);
		std::string buildErrorResponse(int statusCode, int close, const ServerConfig* config, const std::vector<std::string>& s);
};

#endif
