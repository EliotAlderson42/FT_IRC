NAME = irc
SRC = main.cpp Server.cpp Client.cpp Channel.cpp NICK.cpp Password.cpp
OBJ = $(SRC:.cpp=.o)
C = c++
CFLAGS = -g3 -Wall -Wextra -Werror -std=c++98

all: $(NAME)

$(NAME): $(OBJ)
	$(C) $(CFLAGS) -o $(NAME) $(OBJ)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re