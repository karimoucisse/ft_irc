NAME = ircserv
CPP = c++
INC_FOLDER = includes
CPPFLAGS =  -I. -I$(INC_FOLDER)
SRC = srcs/
CMD_FOLDER = $(SRC)cmds/
CMD_FILES = $(CMD_FOLDER)join.cpp $(CMD_FOLDER)topic.cpp $(CMD_FOLDER)kick.cpp
SRCS = $(SRC)main.cpp $(SRC)server.cpp $(SRC)client.cpp $(SRC)channel.cpp $(CMD_FILES)
RM = rm -f
OFILES = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME) : $(OFILES)
	$(CPP) $(CPPFLAGS) $(OFILES) -o $(NAME)
%.o:%.cpp $(INC_FOLDER)/*.hpp
	$(CPP) $(CPPFLAGS) -c $< -o $@
clean:
	$(RM) $(OFILES)
fclean: clean
	$(RM) $(NAME)
re: fclean all
.PHONY: all clean fclean re

