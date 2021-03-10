#ifndef RAYTRACING_H
#define RAYTRACING_H

#include "minilibx/mlx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct	s_point
{
	double x;
	double y;
	double z;
}				t_point;

typedef struct	s_data
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_data;

typedef struct	s_vars
{
	void	*win;
	void	*mlx;
	t_data	data;
}				t_vars;

#endif
