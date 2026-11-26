
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
		int isMethodAllowed(const std::string& method, const RequestConfig& route);
		std::vector<std::string> methodNotAllowedHeaders(const RequestConfig& route);
		std::string handleIncoming(const std::string& s, ServerConfig& config);
		std::string routeRequest(http& input, int c, const RequestConfig& route);
		std::string processGet(http& input, int c, const RequestConfig& route);
		std::string processPost(http& input, int c, const RequestConfig& route);
		std::string processDelete(http& input, int c, const RequestConfig& route);
		std::string buildStandardResponse(int status, const std::string& responseBody, int c);
		std::string buildErrorResponse(int status, int c, const RequestConfig& config);
		std::string buildRedirectResponse(int status, const std::string& redirectTarget, int c);
		std::string buildErrorResponse(int status,int c,const ServerConfig& config);
		std::string buildResponse(const std::string& status, const std::string& responseBody, int c, const std::vector<std::string>& s);
		std::string appendHeaderToResponse(const std::string& response, const std::string& headerLine);
		std::string buildErrorResponse(int status, int c, const RequestConfig& config, const std::vector<std::string>& s);
};

#endif
