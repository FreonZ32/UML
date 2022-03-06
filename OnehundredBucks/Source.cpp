#include<iostream>
#include<math.h>
using namespace std;


void main()
{
	double x, y, z;
	int i = 0;
	while (i != 1000)
	{
		x = 1 + (rand() % 10) / 10.;
		for (int i = 0; i < 3; i++, x = (x + (rand() % 10)) * 0.1);
		y = 1 + (rand() % 10) / 10.;
		for (int i = 0; i < 3; i++, y = (y + (rand() % 10)) * 0.1);
		if ((pow(x, 2) + pow(y, 2)) < 1)
		{
			z = sqrt(1 - (pow(x, 2) + pow(y, 2)));
			if ((z * 10000 - int(z * 10000)) == 0)
			{
				cout << x << endl;
			i++;
			}
		}
	}
}