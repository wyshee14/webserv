#include "engine.hpp"

std::string Engine::handleIncoming(const std::string& s, ServerConfig& config)
{
	size_t maximumBodyLength = config.getClientMaxBodySize();

    http http;

    if (http.process(s, maximumBodyLength))
        return buildErrorResponse(400, 1, config);

    RequestConfig cf(http.getPath(), config);

    int c = http.mustCloseByHttpRules();

    if (!isMethodAllowed(http.getMethod(), cf))
    {
        return buildErrorResponse(405, c, cf, methodNotAllowedHeaders(cf));
    }

    return routeRequest(http, c, cf);
}
