#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <fstream>
using namespace std;

double f(double x, double y, double z, double g)
{
	return (cos(x) * pow(M_E, (2.0 * x)) + 2.0 * g - z + 2.0 * y);
}

double k1(double x, double y, double z, double g)
{
	return f(x, y, z, g);
}
double k2(double x, double y, double z, double g, double h, double k1)
{
	return f((x + h / 3.0), (y + (h/3.0)*k1), z, g);
}
double k3(double x, double y, double z, double g, double h, double k1, double k2)
{
	return f((x + h/3.0), y + (h/6.0)*k1 + (h/6.0)*k2, z, g);
}
double k4(double x, double y, double z, double g, double h, double k1, double k2)
{
	return f(x + h / 2.0, y + h / 8.0 * k1 + (3.0 * h / 8.0)*k2, z, g);
}
double k5(double x, double y, double z, double g, double h, double k1, double k3, double k4)
{
	return f(x + h, y + (h / 2.0)*k1 - (3.0 * h / 2.0)*k3 + 2.0 * h*k4, z, g);
}

double twice(double h, double k1, double k3, double k4)
{
	return (h / 2.0)*(k1 - 3.0 * k3 + 4.0 * k4);
}
double once(double h, double k1, double k4, double k5)
{
	return (h / 6.0)*(k1 + 4.0 * k4 + k5);
}

//double func(double x)
//{
//	return (1.0 / 40)*( 8*pow(M_E, 2*x) + (5*pow(M_E, 2*x) - 11)*sin(x) + (37 - 5*pow(M_E, 2 * x))*cos(x) ) ;
//}

int main()
{
	ofstream fout_y("file_y.txt");// y(x)
	ofstream fout_z("file_z.txt");// y'(x)
	ofstream fout_y1("file_y1.txt");// y'(y)
	ofstream fout_y2("file_y2.txt");// y''(y')
	double x = 0;

	double y = 1; 
	double z = 0; // y'
	double g = 0; // z' = y''

	double yt = 1;
	double zt = 0;
	double gt = 0;
	
	double h; // шаг
	cout << "Enter first step(h): ";
	cin >> h;

	double E = 0.01; // точность

	double R; // погрешность

	int i1 = 0;
	int i2 = 0;

	do
	{
		fout_y << x << "    " << yt << endl;
		fout_z << x << "    " << zt << endl;
		fout_y1 << yt << "    " << zt << endl;
		fout_y2 << zt << "    " << gt << endl;

		y = y + h * z;
		z = z + h * g;
		g = g + twice(h, k1(x, y, z, g), k3(x, y, z, g, h, k1(x, y, z, g), k2(x, y, z, g, h, k1(x, y, z, g))), k4(x, y, z, g, h, k1(x, y, z, g), k2(x, y, z, g, h, k1(x, y, z, g))));

		yt = yt + h * zt;
		zt = zt + h * gt;
		gt = gt + once(h, k1(x, yt, zt, gt), k4(x, yt, zt, gt, h, k1(x, yt, zt, gt), k2(x, yt, zt, gt, h, k1(x, yt, zt, gt))), k5(x, yt, zt, gt, h, k1(x, yt, zt, gt), k3(x, yt, zt, gt, h, k1(x, yt, zt, gt), k2(x, yt, zt, gt, h, k1(x, yt, zt, gt))), k4(x, yt, zt, gt, h, k1(x, yt, zt, gt), k2(x, yt, zt, gt, h, k1(x, yt, zt, gt)))) );

		//cout << "twice:    y = " << y << "    z = " << z << "    g = " << g << endl;
		//cout << "once:    yt = " << yt << "    zt = " << zt << "    gt = " << gt << endl;
		//cout << "h:    " << h << endl;
		//cout << "x:    " << x + h << endl;
		//cout << "------------------------------------------" << endl;

		if (i1 == i2) // уменьшение или увеличение шага делаем после момента, на котором остановились в прошлый раз
		{
			i1++;
		}
		i2++;

		R = 0.2 * abs(gt - g); // погрешность

		if (R > E && i1 == i2)// уменьшение шага
		{
			h /= 2;
			x = 0;
			y = 1;
			z = 0;
			g = 0;
			yt = 1;
			zt = 0;
			gt = 0;
			i2 = 0;
			fout_y.close();
			fout_z.close();
			fout_y1.close();
			fout_y2.close();
			fout_y.open("file_y.txt");
			fout_z.open("file_z.txt");
			fout_y1.open("file_y1.txt");
			fout_y2.open("file_y2.txt");
			continue;
		}
		if ((R <= E / 64.0) && i1 == i2)// увеличение шага
		{
			h *= 2;
			x = 0;
			y = 1;
			z = 0;
			g = 0;
			yt = 1;
			zt = 0;
			gt = 0;
			i2 = 0;
			fout_y.close();
			fout_z.close();
			fout_y1.close();
			fout_y2.close();
			fout_y.open("file_y.txt");
			fout_z.open("file_z.txt");
			fout_y1.open("file_y1.txt");
			fout_y2.open("file_y2.txt");
			continue;
		}

		x = x + h;
		
	} while (x <= 2);
	fout_y << x << "    " << yt << endl;
	fout_z << x << "    " << zt << endl;
	fout_y1 << yt << "    " << zt << endl;
	fout_y2 << zt << "    " << gt << endl;

	cout << "new h = " << h << endl;

	fout_y.close();
	fout_z.close();
	fout_y1.close();
	fout_y2.close();
	system("pause");
	return 0;
}