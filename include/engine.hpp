
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
		int checkPath(const std::string& path);
		int isMethodAllowed(const std::string& method, const RequestConfig* route);
		std::vector<std::string> methodNotAllowedHeaders(const RequestConfig* route);
		std::string handleIncoming(const std::string& req, ServerConfig* cf);
		std::string routeRequest(http& input, int close, const ServerConfig& config, const RequestConfig* route);
		std::string processGet(http& input, int close, const ServerConfig& config, const RequestConfig* route);
		std::string processPost(http& input, int close, const ServerConfig& config, const RequestConfig* route);
		std::string processDelete(http& input, int close, const ServerConfig& config, const RequestConfig* route);
		std::string buildStandardResponse(int statusCode, const std::string& responseBody, int close);
		std::string buildErrorResponse(int statusCode, int close, const RequestConfig& config);
		std::string buildRedirectResponse(int statusCode, const std::string& redirectTarget, int close);
		std::string buildResponse(const std::string& status, const std::string& responseBody, int close, const std::vector<std::string>& s);
		std::string appendHeaderToResponse(const std::string& response, const std::string& headerLine);
		std::string buildErrorResponse(int statusCode, int close, const RequestConfig& config, const std::vector<std::string>& s);
};

#endif
