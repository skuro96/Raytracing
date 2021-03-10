#include "raytracing.h"

#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 800

t_point Point(double x, double y, double z)
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

int close(int keycode, t_vars *vars)
{
	if (keycode == 53)
	{
		mlx_destroy_window(vars->mlx, vars->win);
		exit(0);
	}
	return (0);
}

int main(void)
{
	t_vars vars;
	init_vars(&vars);

	t_point Pv = Point(0, 0, 2000);
	t_point PL = Point(20000, -20000, 20000);
	t_point Pc[2] = {Point(50, -50, -1000), Point(-200, 200, -1700)};
	double R[2] = {200, 200};

	for (int i = 0; i < WINDOW_WIDTH; i++)
	{
		for (int j = 0; j < WINDOW_HEIGHT; j++)
		{
			t_point v = Point(i - WINDOW_WIDTH / 2 + 0.5 - Pv.x, j - WINDOW_HEIGHT / 2 + 0.5 - Pv.y, -Pv.z);

			double t0;
			int s0 = -1;
			for (int s = 0; s < 2; s++)
			{
				t_point Pvc = Point(Pv.x - Pc[s].x, Pv.y - Pc[s].y, Pv.z - Pc[s].z);
				double A = InnerProduct(v, v);
				double B2 = InnerProduct(Pvc, v);
				double C = InnerProduct(Pvc, Pvc) - R[s] * R[s];
				double D = B2 * B2 - A * C;
				if (D >= 0)
				{
					double t = (-B2 - sqrt(D)) / A;
					if (s0 < 0 || t < t0)
					{
						s0 = s;
						t0 = t;
					}
				}
			}

			if (s0 >= 0)
			{
				t_point P = Point(Pv.x + v.x * t0, Pv.y + v.y * t0, Pv.z + v.z * t0);
				t_point N = Point(P.x - Pc[s0].x, P.y - Pc[s0].y, P.z - Pc[s0].z);
				t_point vL = Point(P.x - PL.x, P.y - PL.y, P.z - PL.z);
				
				double cosA = cos_vec(vL, N);

				double w = -InnerProduct(vL, N) / InnerProduct(N, N);
				t_point vR = Point(2 * w * N.x + vL.x, 2 * w * N.y + vL.y, 2 * w * N.z + vL.z);
				double cosG = pow(cos_vec(vR, v) < 0 ? 0 : cos_vec(vR, v), 15);

				double Kd = 1.0, Ks = 0.7, Ie = 0.1, I = 255;

				int color_val[3];
				int RGB[2][3] = {{255, 0, 0}, {0, 255, 0}};
				for (int k = 0; k < 3; k++)
				{
					color_val[k] = RGB[s0][k] * Kd * cosA;
					if (color_val[k] < I * Ks * cosG)
						color_val[k] = I * Ks * cosG;
					if (color_val[k] < RGB[s0][k] * Ie)
						color_val[k] = RGB[s0][k] * Ie;
					draw_pixel(&vars.data, i, j, color(color_val[0], color_val[1], color_val[2]));
				}
			}
			else
			{
				draw_pixel(&vars.data, i, j, color(0, 0, 0));
			}
		}
	}
	mlx_put_image_to_window(vars.mlx, vars.win, vars.data.img, 0, 0);
	mlx_hook(vars.win, 2, 1L<<0, &close, &vars);
	mlx_loop(vars.mlx);
}
