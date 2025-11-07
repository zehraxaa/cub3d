	NAME = cub3d

CC = cc
CFLAGS = -Wall -Wextra -Werror

SRC = main.c utils.c utils1.c map_check.c map_check_walls.c $(GNL_SRCS)

GNL_SRCS = get_next_line/get_next_line.c get_next_line/get_next_line_utils.c
GNL_OBJS = $(GNL_SRCS:.c=.o)

LIBFT = libft/libft.a

OBJ = $(SRC:.c=.o)

all: $(NAME)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	@make -C libft
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME) 
	@echo "✅ $(NAME) compiled"

clean:
	@make clean -C libft
	@rm -f $(OBJ) $(GNL_OBJS)
	@echo "🧹 bye bye object files"

fclean:
	@make fclean -C libft
	@rm -rf $(OBJ)
	@rm -f $(NAME)
	@echo "🧼 fully cleaned up"

re: fclean all

.PHONY: all clean fclean re 