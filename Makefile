NAME = webserv

CC = c++

CFLAGS = -Wall -Wextra -Werror -std=c++98 #-fsanitize=address

SRC = src/main.cpp \
	  src/ConfigParsing/ConfigFile.cpp \
	  src/ConfigParsing/ServerConfig.cpp \
	  src/ConfigParsing/LocationConfig.cpp \
	  src/ConfigParsing/Webserv.cpp

OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.cpp #$(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re