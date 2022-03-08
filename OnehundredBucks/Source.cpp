#include<iostream>
#include<math.h>
#include<fstream>
using namespace std;


void main()
{
	srand(time(NULL));
	ofstream WriteToFile("File.txt");
	double x, y, z;
	int i = 0;
	while (i != 1000)
	{
		x = (rand() % 10000) / 10000.;
		y = (rand() % 10000) / 10000.;
		if (pow(x, 2) + pow(y, 2) < 1 && x!=0 && y!=0)
		{
			z = sqrt(1 - (pow(x, 2) + pow(y, 2)));
			if ((z * 10000 - int(z * 10000)) == 0)
			{
				WriteToFile << x << " " << y << " " << z << endl;
				i++;
			}
		}
	}
	WriteToFile.close();
}