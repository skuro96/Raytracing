#include "raytracing.h"

#define WINDOW_HEIGHT 500
#define WINDOW_WIDTH 500

t_point point(double x, double y, double z)
{
	t_point p;

	p.x = x;
	p.y = y;
	p.z = z;
	return (p);
}

double InnerProduct(t_point a, t_point b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

void init_vars(t_vars *vars)
{
	vars->mlx = mlx_init();
	vars->win = mlx_new_window(vars->mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "Raytracing");
	vars->data.img = mlx_new_image(vars->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	vars->data.addr = mlx_get_data_addr(vars->data.img, &vars->data.bits_per_pixel, &vars->data.line_length, &vars->data.endian);
}

void	draw_pixel(t_data *data, int x, int y, int color)
{
    char    *dst;

    dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
    *(unsigned int*)dst = color;
}

int color(int r, int g, int b)
{
	return (r * 256 * 256 + g * 256 + b);
}

double cos_vec(t_point a, t_point b)
{
	return (-InnerProduct(a, b) / (sqrt(InnerProduct(a, a) * (InnerProduct(b, b)))));
}

int main(void)
{
	t_vars vars;
	init_vars(&vars);

	t_point Pv = point(0, 0, 2000);
	t_point Pc = point(0, 0, -1000);
	t_point PL = point(-20000, -20000, 20000);
	double R = 200;

	double A, B2, C, D;
	t_point Pvc = point(Pv.x - Pc.x, Pv.y - Pc.y, Pv.z - Pc.z);

	t_point v;
	for (int i = 0; i < WINDOW_WIDTH; i++)
	{
		for (int j = 0; j < WINDOW_HEIGHT; j++)
		{
			v.x = i - WINDOW_WIDTH / 2 + 0.5 - Pv.x;
			v.y = j - WINDOW_HEIGHT / 2 + 0.5 - Pv.y;
			v.z = -Pv.z;

			A = InnerProduct(v, v);
			B2 = InnerProduct(Pvc, v);
			C = InnerProduct(Pvc, Pvc) - R * R;
			D = B2 * B2 - A * C;

			double t0 = (-B2 - sqrt(D)) / A;
			t_point P = point(Pv.x + v.x * t0, Pv.y + v.y * t0, Pv.z + v.z * t0);
			t_point N = point(P.x - Pc.x, P.y - Pc.y, P.z - Pc.z);
			t_point vL = point(P.x - PL.x, P.y - PL.y, P.z - PL.z);

			double cosA = cos_vec(vL, N);

			double w = -InnerProduct(vL, N) / InnerProduct(N, N);
			t_point vR = point(2 * w * N.x, 2 * w * N.y, 2 * w * N.z);
			double cosG = cos_vec(vR, v) < 0 ? 0 : cos_vec(vR, v);
			cosG = pow(cosG, 20);

			double Kd = 1.0, Ks = 0.7, Ie = 0.1, I = 255;
			double RGB[3] = {255, 0, 0}, color_val[3];

			if (D >= 0)
			{
				for (int k = 0; k < 3; k++)
				{
					color_val[k] = RGB[k] * Kd * cosA;
					if (color_val[k] < I * Ks * cosG) color_val[k] = I * Ks * cosG;
					if (color_val[k] < RGB[k] * Ie) color_val[k] = RGB[k] * Ie;
				}
				// if (cosA >= 0)
				draw_pixel(&vars.data, i, j, color(color_val[0], color_val[1], color_val[2]));
				// else
				// 	draw_pixel(&vars.data, i, j, color(0, 0, 0));
			}
			else
			{
				draw_pixel(&vars.data, i, j, color(0, 0, 127));
			}
		}
	}
	mlx_put_image_to_window(vars.mlx, vars.win, vars.data.img, 0, 0);
	mlx_loop(vars.mlx);
}