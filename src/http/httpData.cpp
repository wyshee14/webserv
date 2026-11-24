
#include "http.hpp"

int scanChunkedTransferCodingValue(const std::string& value)
{
    std::stringstream stream(toLowerAsciiEngine(value));
    std::string token;

    while (std::getline(stream, token, ','))
    {
        if (trimAsciiEngine(token) == "chunked")
            return 1;
    }

    return 0;
}

int scanChunkedHeaderFields(const std::string& headerCollection)
{
    std::stringstream str(headerCollection);
    std::string line;

    while (std::getline(str, line))
    {
        if (!line.empty() && line[line.size() - 1] == '\r')
        {
            line.erase(line.size() - 1);
        }

        size_t pos = line.find(':');

        if (pos == std::string::npos)
            continue;

        std::string fieldName = toLowerAsciiEngine(line.substr(0, pos));

        std::string fieldValue = trimAsciiEngine(line.substr(pos + 1));

        if (fieldName == "transfer-encoding" && scanChunkedTransferCodingValue(fieldValue))
        {
            return 1;
        }
    }

    return 0;
}

int getChunkLengthLine(const std::string& lengthline,size_t& chunkLength)
{
    std::string token = lengthline;

    size_t pos = token.find(';');

    if (pos != std::string::npos)
        token = token.substr(0,pos);

    token = trimAsciiEngine(token);

    if (token.empty())
        return 0;

    errno = 0;

    char* conversionEnd = NULL;

    unsigned long convertedLength = std::strtoul(token.c_str(), &conversionEnd, 16);

    if (conversionEnd == token.c_str() || *conversionEnd != '\0' || errno == ERANGE)
    {
        return 0;
    }

    chunkLength = static_cast<size_t>(convertedLength);

    return 1;
}

 int measureChunkedPayloadExtent(const std::string& incoming,size_t payloadBeginning,size_t& completePayloadExtent)
{
    size_t p = payloadBeginning;

    while (1)
    {
        size_t pos = incoming.find("\r\n", p);

        if (pos == std::string::npos)
            return 0;

        size_t upcomingChunkLength = 0;

        if (!getChunkLengthLine(incoming.substr(p,pos - p), upcomingChunkLength))
        {
            return 0;
        }

        p = pos + 2;

        if (incoming.size() < p + upcomingChunkLength + 2)
        {
            return 0;
        }

        p += upcomingChunkLength;

        if (incoming.substr(p,2) != "\r\n")
        {
            return 0;
        }

        p += 2;

        if (upcomingChunkLength == 0)
        {
            completePayloadExtent = p;
            return 1;
        }
    }

    return 0;
}

int getChunkedPayloadForHttpRequest(const std::string& amount,size_t maximumPayloadLength,std::string& payload,int* oversizedPayload)
{
    if (oversizedPayload != NULL)
        *oversizedPayload = 0;

    payload.clear();

    size_t p = 0;

    while (1)
    {
        size_t pos = amount.find("\r\n", p);

        if (pos == std::string::npos)
            return 0;

        size_t upcomingChunkLength = 0;

        if (!getChunkLengthLine(amount.substr(p,pos - p),upcomingChunkLength))
        {
            return 0;
        }

        p = pos + 2;

        if (amount.size() < p + upcomingChunkLength + 2)
        {
            return 0;
        }

        if (upcomingChunkLength > 0)
        {
            if (payload.size() + upcomingChunkLength > maximumPayloadLength)
            {
                if (oversizedPayload != NULL)
                    *oversizedPayload = 1;

                return 0;
            }

            payload.append(amount,p,upcomingChunkLength);
        }

        p += upcomingChunkLength;

        if (amount.substr(p,2) != "\r\n")
        {
            return 0;
        }

        p += 2;

        if (upcomingChunkLength == 0)
            break;
    }

    return p == amount.size() ? 1 : 0;
}

 int inspectDecimalPayloadLength(const std::string& headerCollection,size_t& declaredPayloadLength,int& payloadLengthPresent,int* malformedPayloadLength)
{
    std::istringstream str(headerCollection);
    std::string line;

    while (std::getline(str, line))
    {
        if (!line.empty()
            && line[line.size() - 1] == '\r')
        {
            line.erase(line.size() - 1);
        }

        size_t pos = line.find(':');

        if (pos == std::string::npos)
            continue;

        std::string fieldName = toLowerAsciiEngine(line.substr(0, pos));

        std::string fieldValue = trimAsciiEngine(line.substr(pos + 1));

        if (fieldName == "content-length")
        {
            if (payloadLengthPresent == 1)
            {
                if (malformedPayloadLength != NULL)
                    *malformedPayloadLength = 1;

                return 0;
            }

            if (fieldValue.empty())
            {
                if (malformedPayloadLength != NULL)
                    *malformedPayloadLength = 1;

                return 0;
            }

            errno = 0;

            char* p = NULL;

            unsigned long convertedLength = std::strtoul(fieldValue.c_str(),&p, 10);

            if (p == fieldValue.c_str() || *p != '\0' || errno == ERANGE)
            {
                if (malformedPayloadLength != NULL)
                    *malformedPayloadLength = 1;

                return 0;
            }

            declaredPayloadLength = static_cast<size_t>(convertedLength);

            payloadLengthPresent = 1;
        }
    }

    return 1;
}

int extractCompleteHttp(std::string& incoming,std::string& extractedCollection,int* malformedPayloadLength)
{
    if (malformedPayloadLength != NULL)
        *malformedPayloadLength = 0;

    size_t headerPosition = incoming.find("\r\n\r\n");

    if (headerPosition == std::string::npos)
        return 0;

    size_t payloadBeginning = headerPosition + 4;

    std::string headerCollection = incoming.substr(0, headerPosition);

    if (scanChunkedHeaderFields(headerCollection))
    {
        size_t completeCollectionExtent = 0;

        if (!measureChunkedPayloadExtent(incoming,payloadBeginning,completeCollectionExtent))
        {
            return 0;
        }

        extractedCollection = incoming.substr(0, completeCollectionExtent);

        incoming.erase(0, completeCollectionExtent);

        return 1;
    }

    size_t declaredPayloadLength = 0;
    int payloadLengthPresent = 0;

    if (!inspectDecimalPayloadLength(headerCollection,declaredPayloadLength,payloadLengthPresent,malformedPayloadLength))
    {
        return 0;
    }

    size_t completeCollectionExtent = payloadBeginning;

    if (payloadLengthPresent == 1)
        completeCollectionExtent += declaredPayloadLength;

    if (incoming.size() < completeCollectionExtent)
        return 0;

    extractedCollection = incoming.substr(0, completeCollectionExtent);

    incoming.erase(0, completeCollectionExtent);

    return 1;
}
