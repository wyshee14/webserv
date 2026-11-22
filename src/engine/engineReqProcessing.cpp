#include "engine.hpp"

std::string Engine::handleIncoming(const std::string& req, ServerConfig* cf)
{
    if (cf == NULL)
        return buildErrorResponse(500, 1, cf);

    size_t maximumBodyLength = cf->getMaxBodySize();
    size_t maximumRequestLength = maximumBodyLength + MAXIMUM_HEADER_LENGTH;

    if (req.size() > maximumRequestLength)
        return buildErrorResponse(413, 1, cf);

    http http;

    if (http.process(req, maximumBodyLength))
        return buildErrorResponse(400, 1, cf);

    const LocationConfig* dst = NULL;

    int close = http.mustCloseByHttpRules();

    if (!isMethodAllowed(http.getMethod(), dst))
    {
        return buildErrorResponse(405, close, cf, methodNotAllowedHeaders(dst));
    }

    return routeRequest(http, close, *cf, dst);
}
