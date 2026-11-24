#ifndef TEMP_HPP
# define TEMP_HPP

# include <iostream>
# include <cstdlib>

enum parserstate {
	READING_HEADER,
	READING_BODY,
	RESPONSE,
	ERROR
};

class temp {
	private:
		parserstate _state;
		std::string _header;
		std::string _body;
		size_t _content_length;
	public:
		temp();
		void setParserState(parserstate state);
		parserstate getParserState();
		parserstate engine(std::string& write_buffer);
		parserstate parsing(std::string& read_buffer);
		~temp();

};


#endif