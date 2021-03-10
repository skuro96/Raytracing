all:
		gcc -L minilibx -lmlx -framework OpenGL -framework AppKit main.c
		./a.out

wall:
		gcc -L minilibx -lmlx -framework OpenGL -framework AppKit main.c -Wall -Wextra -Werror
		./a.out