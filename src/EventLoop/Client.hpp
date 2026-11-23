#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include "temp.hpp"
# include <ctime>

//!header without body
// GET /index.html HTTP/1.1\r\n
// Host: localhost:8080\r\n
// User-Agent: curl/8.1.2\r\n
// Accept: */*\r\n
// \r\n 

//!header with body
// POST /api/login HTTP/1.1\r\n
// Host: localhost:8080\r\n
// Content-Type: application/x-www-form-urlencoded\r\n
// Content-Length: 27\r\n
// \r\n
// username=alex&password=1234

//!contiguous requests pipeline
// GET /page1 HTTP/1.1\r\n
// Host: localhost\r\n\r\n
// POST /submit HTTP/1.1\r\n
// Host: localhost\r\n
// Content-Length: 5\r\n
// \r\n
// hello

//!contiguous requests pipeline (flipped)
// POST /submit HTTP/1.1\r\n
// Host: localhost\r\n
// Content-Length: 5\r\n
// \r\n
// helloGET /page1 HTTP/1.1\r\n
// Host: localhost\r\n
// \r\n


//State transition table: READING *handled inside client* (READING_HEADER/READING_BODY)*handled inside http*
//      IDLE, WRITING
//last_activity is the last time stamp (std::time)

//!implement a working client class containing read/write buffer
enum state {
	READING,
	IDLE,
	WRITING,
	CLOSED
};

class Client {
    private:
		state announce;
		int requestCount;
        std::string recvBuffer; //to store read TCP bytes
        std::string sendBuffer; //to store program response
		temp parser;
        //client_body_timeout - a time set between two successive read operation (60s)
        //client_header_timeout - a time set for the entire header read duration (60s)
        //send_timeout - a time set between two successive write operation (60s)
        //keepalive_timeout - a time set for idle period in between requests (75s)
        //keepalive_request - a maximum amount requests in one client connection (100)
    public:
        Client();
		void processBytes();
		void setState(state annc);
		void incrementCount(int count);
		const state& getState();
		std::string& getRecvBuffer();
		std::string& getSendBuffer();
        ~Client();
};

#endif  