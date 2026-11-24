#include "temp.hpp"

temp::temp() : _state(READING_HEADER) {
	std::cout << "temp object created" << std::endl;
}

void temp::setParserState(parserstate state) {
	this->_state = state;
}

parserstate temp::getParserState() {
	return (_state);
}

parserstate temp::parsing(std::string& recv_buffer) {
    // 1. HEADER STAGE: Look for the blank line "\r\n\r\n"
    if (_state == READING_HEADER)
	{
		std::cout << "recv_buffer: " << recv_buffer << std::endl;
        size_t header_end = recv_buffer.find("\r\n\r\n");
        if (header_end == std::string::npos) 
		{
			std::cout << "i got return npos" << std::endl;
            return READING_HEADER; // Incomplete header, wait for more recv() bytes
    	}

        // Extract raw header block (everything before "\r\n\r\n")
        std::string raw_headers = recv_buffer.substr(0, header_end);
		_header = raw_headers;
        
        // Erase headers + the 4-byte "\r\n\r\n" delimiter from recv_buffer
        recv_buffer.erase(0, header_end + 4);
        // Decide if we need to read a body or if we are already done
        if (_header.find("Content-Length") != _header.size())
		{
			std::string raw_content = _header.substr(_header.find("Content-Length: ") + 16, _header.size() - 17);
            _content_length = std::strtoul(raw_content.c_str(), NULL, 10);
            _state = (_content_length > 0) ? READING_BODY : RESPONSE;
        }
		else
		{
			return RESPONSE;// No body (e.g., standard GET)
		}
	}

    // 2. BODY STAGE: Consume exact _content_length bytes from recv_buffer
    if (_state == READING_BODY) //!content length has a number but not content
	{
		std::cout << "_content_length: " << _content_length << std::endl;
		std::cout << recv_buffer.size() << std::endl;
        if (recv_buffer.size() < _content_length)
		{
            return READING_BODY; // Incomplete body, wait for more recv() bytes
        }

        // Copy exact 27 bytes into _body and erase them from recv_buffer
        _body = recv_buffer.substr(0, _content_length);
        recv_buffer.erase(0, _content_length);
        _state = READING_HEADER;
		return RESPONSE;
    }

    return _state;
}

parserstate temp::engine(std::string& write_buffer) {
	std::cout << "*engine* before append: " << write_buffer << std::endl;
	std::cout << "_header: " << _header << std::endl;
	std::cout << "_body: " << _body << std::endl;
	write_buffer.append("-----RESPONSE SUCCESSFUL----\n");
	write_buffer.append(_header);
	if (_body.size() > 0)
		write_buffer.append(_body);
	write_buffer.append("\n");
	std::cout << "engine: " << write_buffer << std::endl;
	_header.clear();
	_body.clear();
	return _state;
}

temp::~temp() {
	std::cout << "temp object destroyed" << std::endl;
}