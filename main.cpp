# include <iostream>

int main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cerr << "Usage: ./config"
	}
}

// Psedocode
// 1. Tokenizer
// - read the file and convert to tokens
// std::vector<Token> tokenize(const std::string& input) {
    // skip whitespace
    // skip comments starting with #
    // detect braces { and }
    // detect semicolons ;
    // read words and numbers
    // return token vector
// }
// 2. Parse by nesting blocks
// - use recursive descent parser
// Grammar idea:
// config        := directive* server_block*
// server_block  := "server" "{" server_directive* location_block* "}"
// location_block:= "location" path "{" location_directive* "}"
// directive    := key value* ";"
// 3. Parse the directives
// - store each private attribute values
// - validate values
// 4. Match location path 
// - exact match = /path
// - prefix math /path
// - default /
