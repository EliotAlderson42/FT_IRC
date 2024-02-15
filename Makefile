NAME = irc
SRC_FILES = $(addprefix srcs/, main.cpp Server.cpp Client.cpp Channel.cpp utils.cpp)
OBJS		= $(SRC_FILES:%.cpp=$(OBJS_DIR)/%.o)
C = c++

# all: $(NAME)

# $(NAME): $(OBJ)
# 	$(C) $(CFLAGS) -o $(NAME) $(OBJ)

# clean:
# 	rm -f $(OBJ)

# fclean: clean
# 	rm -f $(NAME)

# re: fclean all

# .PHONY: all clean fclean re

OBJS_DIR	= .objets


CC			= c++

CFLAGS		= -g3   -Iincludes #-std=c++98 -Wall -Wextra -Werror -Iincludes

all : $(NAME)

$(NAME) : $(OBJS_DIR) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	
$(OBJS_DIR) :
	@mkdir $(OBJS_DIR)
	@mkdir $(OBJS_DIR)/srcs

$(OBJS) : $(OBJS_DIR)/%.o : %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -rf $(NAME)
	
re : fclean all

.PHONY: all clean fclean re