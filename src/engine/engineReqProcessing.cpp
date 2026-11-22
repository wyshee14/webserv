#include "engine.hpp"

std::string Engine::handleIncoming(const std::string& req, ServerConfig* config)
{
    if (config == NULL)
        return buildErrorResponse(500, 1, config);

    size_t maximumBodyLength = config->getMaxBodySize();
    size_t maximumRequestLength = maximumBodyLength + MAXIMUM_HEADER_LENGTH;

    if (req.size() > maximumRequestLength)
        return buildErrorResponse(413, 1, config);

    http http;

    if (http.process(req, maximumBodyLength))
        return buildErrorResponse(400, 1, config);

    const LocationConfig* dst = NULL;

    int close = http.mustCloseByHttpRules();

    if (!isMethodAllowed(http.getMethod(), dst))
    {
        return buildErrorResponse(405, close, config, methodNotAllowedHeaders(dst));
    }

    return routeRequest(http, close, *config, dst);
}
