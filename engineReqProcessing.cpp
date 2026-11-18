
#include "engine.hpp"

int Engine::handleRequestExtraction(Con* c,std::string& req,int& ext)
{
    ext = RESULT_FAILURE;

    std::string& incoming = c->incoming();
    int malformedLength = RESULT_FAILURE;

    if (!httpRequestCompletenessChecking(incoming,req,&malformedLength))
    {
        if (malformedLength == RESULT_SUCCESS)
        {
            c->markClosing(RESULT_SUCCESS);
            c->putReply(
              buildErrorResponse(HTTP_BAD_REQUEST,c->mustClose(),c->getConfig()));
            c->mode(RESP);
            return RESULT_FAILURE;
        }

        size_t pos = incoming.find(HEADER);

        if (pos == std::string::npos)
            c->markRequest(CHECK_HEADER);
        else
            c->markRequest(CHECK_BODY);

        return RESULT_SUCCESS;
    }

    c->markRequest(CHECK_BODY);
    ext = RESULT_SUCCESS;

    return RESULT_SUCCESS;
}

int Engine::handleRequestParsing(Con* c,std::string& req,HttpRequest& request,Config* cf)
{
    size_t maximumBodyLength = MAX_REQUEST_SIZE;

    if (cf != NULL)
        maximumBodyLength = cf->getMaxBodySize();

    try
    {
        request.parse(req, maximumBodyLength);
    }
    catch (cst std::exception& e)
    {
        c->markClosing(RESULT_SUCCESS);

        if (std::string(e.what()) == "Body too large")
        {
            c->putReply(
              buildErrorResponse(HTTP_CONTENT_LARGE,c->mustClose(),cf));
        }
        else
        {
            c->putReply(
              buildErrorResponse(HTTP_BAD_REQUEST,c->mustClose(),cf));
        }

        c->mode(RESP);
        return RESULT_FAILURE;
    }

    return RESULT_SUCCESS;
}

int Engine::handleRequestExecution(Con* c,HttpRequest& request,Config* cf,LocationConfig* dst, int closing, int& replyGenerated)
{
    if (cf == NULL)
    {
        c->markClosing(RESULT_SUCCESS);
        c->putReply(
          buildErrorResponse(HTTP_IN_FAILURE,c->mustClose(),c->getConfig()));
        c->mode(RESP);
        return RESULT_FAILURE;
    }

    if (!isMethodAllowed(request.getMethod(), dst))
    {
        std::string reply = buildErrorResponse(
          HTTP_METHOD_REJECTED,c->mustClose(),cf,methodNotAllowedHeaders(dst));
        c->putReply(reply);
        c->mode(RESP);
        return RESULT_FAILURE;
    }

    std::string reply = routeRequest(request,closing,*cf,dst);

    c->addReply(reply);
    replyGenerated = RESULT_SUCCESS;

    return RESULT_SUCCESS;
}

int Engine::attemptIncomingHeader(Con* c,char* incomingBytes,ssize_t incomingLength)
{
    c->addIncoming(incomingBytes,incomingLength);
    c->updateLastActivity();

    std::string& incoming = c->incoming();

    size_t pos = incoming.find(HEADER);

    if (!enforceRequestSizeLimits(c,pos))
        return RESULT_FAILURE;

    if (pos == std::string::npos)
        c->markRequest(CHECK_HEADER);
    else
        c->markRequest(CHECK_BODY);

    return RESULT_SUCCESS;
}

int Engine::processPendingRequests(Con* c,int& replyGenerated)
{
    std::string req;

    while (RESULT_SUCCESS)
    {
        int ext = RESULT_FAILURE;

        if (!handleRequestExtraction(c,req,ext))
        {
            return RESULT_FAILURE;
        }

        if (ext == RESULT_FAILURE)
            break;

        HttpRequest request;

        Config* cf = c->getConfig();

        if (!handleRequestParsing(c,req,request,cf))
        {
            return RESULT_FAILURE;
        }

        cst Locationcfig* dst = NULL;

        if (cf != NULL)
        {
            dst = findBestLocation(*cf,request.getPath());
        }

        int closing = request.mustCloseByHttpRules();

        if (closing == RESULT_SUCCESS)
            c->markClosing(RESULT_SUCCESS);

        if (!handleRequestExecution(c,request,cf,dst,closing,replyGenerated))
        {
            return RESULT_FAILURE;
        }
    }

    return RESULT_SUCCESS;
}

int Engine::enforceRequestSizeLimits(Con* c,size_t pos)
{
    std::string& incoming = c->incoming();

    cst Config* cf = c->getConfig();

    size_t maximumBodyLength = MAX_REQUEST_SIZE;

    if (cf != NULL)
        maximumBodyLength = cf->getMaxBodySize();

    size_t maximumRequestLength = maximumBodyLength + MAXIMUM_HEADER_LENGTH;

    if (pos == std::string::npos && incoming.size() > MAXIMUM_HEADER_LENGTH)
    {
        c->markClosing(RESULT_SUCCESS);
        incoming.clear();

        c->putReply(buildErrorResponse(HTTP_CONTENT_LARGE,c->mustClose(),cf));

        c->mode(RESP);
        return RESULT_FAILURE;
    }

    if (incoming.size() > maximumRequestLength)
    {
        c->markClosing(RESULT_SUCCESS);
        incoming.clear();

        c->putReply(
          buildErrorResponse(HTTP_CONTENT_LARGE,c->mustClose(),cf));

        c->mode(RESP);
        return RESULT_FAILURE;
    }

    return RESULT_SUCCESS;
}

void Engine::handleIncomingRequest(Con* c,char* incomingBytes,ssize_tincomingLength)
{
    if (!attemptIncomingHeader(c,incomingBytes,incomingLength))
    {
        return;
    }

    int replyGenerated = RESULT_FAILURE;

    if (!processPendingRequests(c,replyGenerated))
        return;

    if (replyGenerated == RESULT_SUCCESS)
        c->mode(RESP);
    else
        c->mode(c_READING_MODE);
}
