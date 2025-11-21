NAME = ircserv
CPP = c++
INC_FOLDER = includes
CPPFLAGS =  -I. -I$(INC_FOLDER)
SRC = srcs/
SRCS = $(SRC)main.cpp
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

