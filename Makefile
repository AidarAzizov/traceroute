NAME = ft_traceroute


FLAGS = -Wall -Wextra -Werror

FUNC = helpers main packet parse traceroute prepare print sendrecv socket lib

HEADER = -I includes/

SRC = $(addprefix srcs/, $(addsuffix .c, $(FUNC)))

OBJ = $(addprefix build/, $(addsuffix .o, $(FUNC)))


all: .PHONY $(NAME)

.PHONY:
	@mkdir -p build

$(NAME): $(OBJ)
	@gcc $(FLAGS) -o $(NAME) $(OBJ) -lm $(HEADER)
	@echo "		(•̀ᴗ•́)و $(NAME) generated!"

build/%.o: srcs/%.c includes/ft_traceroute.h
	@gcc $(FLAGS) -o $@ -lm -c $< $(HEADER)

clean:
	@/bin/rm -f $(OBJ)
	@rm -rf build
	@echo "		OBJ files have been deleted."

fclean: clean
	@/bin/rm -f $(NAME)
	@echo "		$(NAME) have been deleted."

re: fclean all
