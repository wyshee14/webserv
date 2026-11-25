#include "engine.hpp"

std::string Engine::handleIncoming(const std::string& req, ServerConfig* config)
{
    if (config == NULL)
        return buildErrorResponse(500, 1, config);

    http http;

    if (http.process(req, maximumBodyLength))
        return buildErrorResponse(400, 1, config);

    RequestConfig cf(request.getPath(), *config);

    int close = http.mustCloseByHttpRules();

    if (!isMethodAllowed(http.getMethod(), cf))
    {
        return buildErrorResponse(405, close, cf, methodNotAllowedHeaders(cf));
    }

    return routeRequest(http, close, cf);
}
