#include<iostream>
using namespace std;
using std::cout;
using std::cin;
using std::endl;

#define MAX_TANK_VOLUME 80
#define MIN_TANK_VOLUME 40
#define MIN_ENGINE_CONSUMPTION 4
#define MAX_ENGINE_CONSUMPTION 40

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

class Engine
{
	double consumption;
	double consumption_per_second;
	bool EngineWork;

public:
	double get_consumption()const
	{return consumption;}
	double get_consumption_fer_second()const
	{return consumption_per_second;}
	bool get_EngineWork()const
	{return EngineWork;}
	void set_consumption(double consumption)
	{
		if (consumption >= MIN_ENGINE_CONSUMPTION && consumption <= MAX_ENGINE_CONSUMPTION)
			this->consumption = consumption;
		else
			this->consumption = MAX_ENGINE_CONSUMPTION / 2;
		consumption_per_second = consumption * .3e-4;
	}
	void set_EngineWork(bool EngineWork)
	{this->EngineWork = EngineWork;}

	explicit Engine(double consumption)
	{
		set_consumption(consumption);
		set_EngineWork(false);
		cout << "Engine is ready:" << endl;
	}
	~Engine()
	{cout << "Engine is gone:" << endl;}

	bool start()
	{EngineWork = true;}
	bool stop()
	{EngineWork = false;}

	void info()const
	{
		cout << "Consumption:  " << consumption << endl;
		cout << "Consumption per second:  " << consumption_per_second << endl;
		cout << "Engine is:  " << (EngineWork? "started" : "stopped") << endl;
	}
};

//#define TANK_CH
#define ENGINE_CH

void main()
{
	setlocale(LC_ALL, "rus");
#ifdef TANK_CH
	Tank tank(120);
	tank.info();
	int fuel;
	while (true)
	{
		cout << "ֲגוהטעו מבתול: "; cin >> fuel;
		tank.fill(fuel);
		tank.info();
	}
#endif // TANK_CH

#ifdef ENGINE_CH
	Engine engine(9);
	engine.info();
#endif // ENGINE_CH


}