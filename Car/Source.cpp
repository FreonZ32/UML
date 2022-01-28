#include<iostream>
using namespace std;
using std::cout;
using std::cin;
using std::endl;

#define MAX_TANK_VOLUME 80
#define MIN_TANK_VOLUME 40

class Tank
{
	const unsigned int VOLUME;
	double fuel_level;
public:
	const unsigned int get_VOLUME()const
	{return VOLUME;}
	double get_fuel_level()const
	{return fuel_level;}

	Tank(unsigned int volume) :VOLUME(volume >= MIN_TANK_VOLUME && volume <= MAX_TANK_VOLUME ? volume:MAX_TANK_VOLUME), fuel_level(0)
	{
		cout << "Tank is ready\t" << endl;
	}
	~Tank()
	{
		cout << "Tank is gone\t" << endl;
	}

	double fill(double fuel)
	{
		if (fuel_level + fuel <= 0)return fuel_level = 0;
		else if (fuel_level + fuel >= VOLUME)return fuel_level = VOLUME;
		else return fuel_level += fuel;
	}
	double give_fuel(double fuel)
	{
		fuel_level -= fuel;
		if (fuel_level < 0)fuel_level = 0;
		return fuel_level;
	}

	void info()const
	{
		cout << "Tank volume: " << VOLUME << endl;
		cout << "Fuel level: " << fuel_level << endl;
	}
};

void main()
{
	setlocale(LC_ALL, "rus");
	Tank tank(120);
	tank.info();
	int fuel;
	while (true)
	{
		cout << "ֲגוהטעו מבתול: "; cin >> fuel;
		tank.fill(fuel);
		tank.info();
	}
}