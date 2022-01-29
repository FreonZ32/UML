#include<iostream>
#include<Windows.h>
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
	bool IsOnReserve;
	bool IsComplete;
public:
	const unsigned int get_VOLUME()const
	{return VOLUME;}
	double get_fuel_level()const
	{return fuel_level;}
	bool get_IsComplete()
	{return IsComplete;}
	Tank(unsigned int volume = MAX_TANK_VOLUME)
		:VOLUME(volume >= MIN_TANK_VOLUME && volume <= MAX_TANK_VOLUME ? volume : MAX_TANK_VOLUME), fuel_level(0),IsOnReserve(true), IsComplete (true)
	{
		cout << "Tank is ready\t" << endl;
	}
	~Tank()
	{
		cout << "Tank is gone\t" << endl;
	}

	double fill(double fuel)
	{
		if (fuel_level + fuel <= 0) { fuel_level = 0; ChecksSwitch(fuel_level); return fuel_level; }
		else if (fuel_level + fuel >= VOLUME) { fuel_level = VOLUME; ChecksSwitch(fuel_level); return fuel_level; }
		else { fuel_level += fuel; ChecksSwitch(fuel_level); return fuel_level; };
	}
	double give_fuel(double fuel)
	{
		fuel_level -= fuel;
		if (fuel_level < 0)fuel_level = 0;
		ChecksSwitch(fuel_level);
		return fuel_level;
	}
	void ChecksSwitch(double fuel_level)
	{
		fuel_level <= 5 ? this->IsOnReserve = true : this->IsOnReserve = false;
		fuel_level == 0 ? this->IsComplete = true : this->IsComplete = false;
	}
	void info()const
	{
		cout << "Tank volume: " << VOLUME << endl;
		cout << "Fuel level: " << fuel_level << endl;
		if (IsComplete)cout << "Fuel level on zero!" << endl;
		else if(IsOnReserve)cout << "Fuel level is low!" << endl;
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

	explicit Engine(double consumption = MAX_ENGINE_CONSUMPTION)
	{
		set_consumption(consumption);
		this->EngineWork = false;
		cout << "Engine is ready:" << endl;
	}
	~Engine()
	{cout << "Engine is gone:" << endl;}


	void start()
	{EngineWork = true;}
	void stop()
	{EngineWork = false;}

	void info()const
	{
		cout << "Consumption:  " << consumption << endl;
		cout << "Consumption per second:  " << consumption_per_second << endl;
		cout << "Engine is:  " << (EngineWork? "started" : "stopped") << endl;
	}
};

class Car 
{
	Engine CEngine;
	Tank CTank;
public:
	Car(double fuel_level, double consumption)
	{
		this->CTank.fill(fuel_level);
		this->CEngine.set_consumption(consumption);
	}
	~Car(){}

	void CarOn()
	{
		if(CTank.get_IsComplete()) { AllInfo(); return; }
		CEngine.start();
		while(!CTank.get_IsComplete())
		{
			CTank.give_fuel(CEngine.get_consumption_fer_second());
			system("cls");
			AllInfo();
			Sleep(100);
		}
	}
	void AllInfo()
	{
		CEngine.info();
		CTank.info();
	}	
	//friend class Engine;
	//friend class Tank;
};

//#define TANK_CH
//#define ENGINE_CH
#define CAR_CH

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

#ifdef CAR_CH
	Car car1(5, 9);
	car1.CarOn();
#endif // CAR_CH

}