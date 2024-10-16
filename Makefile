

NAME = cub3D
LIBMLX = ../../minilibx/libmlx_Linux.a
SRC_DIR = src
FLAGSMLX = -Lmlx -lXext -lX11 -lglfw -lm
CFLAGS =  -Wall -Werror -Wextra
CC  = cc -g
RM = rm -rf
SRCS = $(wildcard $(SRC_DIR)/*.c) 
LIBFT = libft.a



OBJ = ${SRCS:.c=.o}

all: $(NAME)

$(NAME) : $(OBJ)
	@make -C libft
	@ $(CC) $(CFLAGS) $(OBJ) $(LIBMLX) $(FLAGSMLX) libft/$(LIBFT) -o $(NAME)
	@echo Compilation Finish

clean: 
	@make clean -C libft
	${RM} ${OBJ}

fclean: clean
	@make fclean -C libft
	${RM} $(NAME)
	${RM} $(LIBFT)

re: fclean all
