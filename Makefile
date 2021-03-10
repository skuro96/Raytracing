all:
		gcc -L minilibx -lmlx -framework OpenGL -framework AppKit main.c