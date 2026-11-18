
#include "request.hpp"

int scanChunkedTransferCodingValue(const std::string& value)
{
    std::istringstream stream(toLowerAsciiEngine(value));
    std::string token;

    while (std::getline(stream, token, ','))
    {
        if (trimAsciiEngine(token) == "chunked")
            return SUCCESS_RESULT;
    }

    return FAILURE_RESULT;
}

int scanChunkedHeaderFields(const std::string& headerCollection)
{
    std::istringstream headerStream(headerCollection);
    std::string headerLine;

    while (std::getline(headerStream, headerLine))
    {
        if (!headerLine.empty() && headerLine[headerLine.size() - 1] == '\r')
        {
            headerLine.erase(headerLine.size() - 1);
        }

        size_t pos = headerLine.find(':');

        if (pos == std::string::npos)
            continue;

        std::string fieldName = toLowerAsciiEngine(headerLine.substr(EMPTY_POSITION, pos));

        std::string fieldValue = trimAsciiEngine(headerLine.substr(pos + 1));

        if (fieldName == "transfer-encoding" && scanChunkedTransferCodingValue(fieldValue))
        {
            return SUCCESS_RESULT;
        }
    }

    return FAILURE_RESULT;
}

int getChunkLengthLine(const std::string& lengthline,size_t& chunkLength)
{
    std::string hexadecimalToken = lengthline;

    size_t extensionPosition = hexadecimalToken.find(';');

    if (extensionPosition != std::string::npos)
        hexadecimalToken = hexadecimalToken.substr(EMPTY_POSITION,extensionPosition);

    hexadecimalToken = trimAsciiEngine(hexadecimalToken);

    if (hexadecimalToken.empty())
        return FAILURE_RESULT;

    errno = NO_ERROR;

    char* conversionEnd = NULL;

    unsigned long convertedLength = std::strtoul(hexadecimalToken.c_str(),&conversionEnd,HEXADECIMAL_RADIX);

    if (conversionEnd == hexadecimalToken.c_str() ||*conversionEnd != STRING_END_MARKER ||errno == ERANGE)
    {
        return FAILURE_RESULT;
    }

    chunkLength = static_cast<size_t>(convertedLength);

    return SUCCESS_RESULT;
}

 int measureChunkedPayloadExtent(const std::string& incoming,size_t payloadBeginning,size_t& completePayloadExtent)
{
    size_t p = payloadBeginning;

    while (CONTINUE_PROCESSING)
    {
        size_t pos = incoming.find(HTTP_LINE, p);

        if (pos == std::string::npos)
            return FAILURE_RESULT;

        size_t upcomingChunkLength = EMPTY_POSITION;

        if (!getChunkLengthLine(incoming.substr(p,pos - p),upcomingChunkLength))
        {
            return FAILURE_RESULT;
        }

        p = pos + LINE_LENGTH;

        if (incoming.size() < p + upcomingChunkLength + LINE_LENGTH)
        {
            return FAILURE_RESULT;
        }

        p += upcomingChunkLength;

        if (incoming.substr(p,LINE_LENGTH) != HTTP_LINE)
        {
            return FAILURE_RESULT;
        }

        p += LINE_LENGTH;

        if (upcomingChunkLength == EMPTY_POSITION)
        {
            completePayloadExtent = p;
            return SUCCESS_RESULT;
        }
    }

    return FAILURE_RESULT;
}

int getChunkedPayloadForHttpRequest(const std::string& amount,size_t maximumPayloadLength,std::string& payload,int* oversizedPayload)
{
    if (oversizedPayload != NULL)
        *oversizedPayload = FAILURE_RESULT;

    payload.clear();

    size_t p = EMPTY_POSITION;

    while (CONTINUE_PROCESSING)
    {
        size_t pos = amount.find(HTTP_LINE, p);

        if (pos == std::string::npos)
            return FAILURE_RESULT;

        size_t upcomingChunkLength = EMPTY_POSITION;

        if (!getChunkLengthLine(amount.substr(p,pos - p),upcomingChunkLength))
        {
            return FAILURE_RESULT;
        }

        p = pos + LINE_LENGTH;

        if (amount.size() < p + upcomingChunkLength + LINE_LENGTH)
        {
            return FAILURE_RESULT;
        }

        if (upcomingChunkLength > EMPTY_POSITION)
        {
            if (payload.size() + upcomingChunkLength > maximumPayloadLength)
            {
                if (oversizedPayload != NULL)
                    *oversizedPayload = SUCCESS_RESULT;

                return FAILURE_RESULT;
            }

            payload.append(amount,p,upcomingChunkLength);
        }

        p += upcomingChunkLength;

        if (amount.substr(p,LINE_LENGTH) != HTTP_LINE)
        {
            return FAILURE_RESULT;
        }

        p += LINE_LENGTH;

        if (upcomingChunkLength == EMPTY_POSITION)
            break;
    }

    return p == amount.size() ? SUCCESS_RESULT : FAILURE_RESULT;
}

 int inspectDecimalPayloadLength(const std::string& headerCollection,size_t& declaredPayloadLength,int& payloadLengthPresent,int* malformedPayloadLength)
{
    std::istringstream headerStream(headerCollection);
    std::string headerLine;

    while (std::getline(headerStream, headerLine))
    {
        if (!headerLine.empty()
            && headerLine[headerLine.size() - 1] == '\r')
        {
            headerLine.erase(headerLine.size() - 1);
        }

        size_t pos = headerLine.find(':');

        if (pos == std::string::npos)
            continue;

        std::string fieldName = toLowerAsciiEngine(headerLine.substr(EMPTY_POSITION, pos));

        std::string fieldValue = trimAsciiEngine(headerLine.substr(pos + 1));

        if (fieldName == "content-length")
        {
            if (payloadLengthPresent == SUCCESS_RESULT)
            {
                if (malformedPayloadLength != NULL)
                    *malformedPayloadLength = SUCCESS_RESULT;

                return FAILURE_RESULT;
            }

            if (fieldValue.empty())
            {
                if (malformedPayloadLength != NULL)
                    *malformedPayloadLength = SUCCESS_RESULT;

                return FAILURE_RESULT;
            }

            errno = NO_ERROR;

            char* conversionEnd = NULL;

            unsigned long convertedLength = std::strtoul(fieldValue.c_str(),&conversionEnd,DECIMAL_RADIX);

            if (conversionEnd == fieldValue.c_str() ||*conversionEnd != STRING_END_MARKER || errno == ERANGE)
            {
                if (malformedPayloadLength != NULL)
                    *malformedPayloadLength = SUCCESS_RESULT;

                return FAILURE_RESULT;
            }

            declaredPayloadLength = static_cast<size_t>(convertedLength);

            payloadLengthPresent = SUCCESS_RESULT;
        }
    }

    return SUCCESS_RESULT;
}

int extractCompleteHttp(std::string& incoming,std::string& extractedCollection,int* malformedPayloadLength)
{
    if (malformedPayloadLength != NULL)
        *malformedPayloadLength = FAILURE_RESULT;

    size_t headerPosition = incoming.find(HTTP_HEADER);

    if (headerPosition == std::string::npos)
        return FAILURE_RESULT;

    size_t payloadBeginning = headerPosition + HEADER_LENGTH;

    std::string headerCollection = incoming.substr(EMPTY_POSITION, headerPosition);

    if (scanChunkedHeaderFields(headerCollection))
    {
        size_t completeCollectionExtent = EMPTY_POSITION;

        if (!measureChunkedPayloadExtent(incoming,payloadBeginning,completeCollectionExtent))
        {
            return FAILURE_RESULT;
        }

        extractedCollection =
            incoming.substr(EMPTY_POSITION, completeCollectionExtent);

        incoming.erase(EMPTY_POSITION, completeCollectionExtent);

        return SUCCESS_RESULT;
    }

    size_t declaredPayloadLength = EMPTY_POSITION;
    int payloadLengthPresent = FAILURE_RESULT;

    if (!inspectDecimalPayloadLength(headerCollection,declaredPayloadLength,payloadLengthPresent,malformedPayloadLength))
    {
        return FAILURE_RESULT;
    }

    size_t completeCollectionExtent = payloadBeginning;

    if (payloadLengthPresent == SUCCESS_RESULT)
        completeCollectionExtent += declaredPayloadLength;

    if (incoming.size() < completeCollectionExtent)
        return FAILURE_RESULT;

    extractedCollection = incoming.substr(EMPTY_POSITION, completeCollectionExtent);

    incoming.erase(EMPTY_POSITION, completeCollectionExtent);

    return SUCCESS_RESULT;
}
