NAME = irc
SRC_FILES = $(wildcard srcs/*.cpp )\
			$(wildcard srcs/commands/*.cpp)
OBJS		= $(SRC_FILES:%.cpp=$(OBJS_DIR)/%.o)
C = c++

OBJS_DIR	= .objets


CC			= c++

CFLAGS		= -g3 -Iincludes -std=c++98 -Wall -Wextra -Werror

all : $(NAME)

$(NAME) : $(OBJS_DIR) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	
$(OBJS_DIR) :
	@mkdir $(OBJS_DIR)
	@mkdir $(OBJS_DIR)/srcs
	@mkdir $(OBJS_DIR)/srcs/commands

$(OBJS) : $(OBJS_DIR)/%.o : %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -rf $(NAME)
	
re : fclean all

.PHONY: all clean fclean re