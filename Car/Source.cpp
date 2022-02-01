#include<iostream>
#include<conio.h>
#include<Windows.h>

using namespace std;
using std::cout;
using std::cin;
using std::endl;

#define MAX_TANK_VOLUME 80
#define MIN_TANK_VOLUME 40
#define MIN_ENGINE_CONSUMPTION 4
#define MAX_ENGINE_CONSUMPTION 40
#define MIN_GEAR_SHIFT 0
#define MAX_GEAR_SHIFT 8000

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
		:VOLUME(volume >= MIN_TANK_VOLUME && volume <= MAX_TANK_VOLUME ? volume : MAX_TANK_VOLUME),
		fuel_level(0),IsOnReserve(true), IsComplete (true)
	{cout << "Tank is ready\t" << endl;}
	~Tank()
	{cout << "Tank is gone\t" << endl;}

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
		printf("Tank volume: %u\n", VOLUME);
		printf("Fuel level: %2.3f\n", fuel_level);
		if (IsComplete)printf("Fuel level on zero!\n");
		else if(IsOnReserve)printf("Fuel level is low!\n");
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
		//cout << "Engine is ready:" << endl;
	}
	~Engine()
	{/*cout << "Engine is gone:" << endl;*/}

	void start()
	{EngineWork = true;}
	void stop()
	{EngineWork = false;}

	double ConsumptionWithSpeed(unsigned short engineSpeed)
	{
		if (engineSpeed == 0)return 0;
		else return((double(engineSpeed-1500))/1000000);
	}

	void info(double counsumption_per_second_plus_speedEngine)const
	{
		printf("Consumption: %f\n", consumption);
		printf("Consumption per second: %2.5f\n", consumption_per_second + counsumption_per_second_plus_speedEngine);
		cout << "Engine is: " << (EngineWork?"started" : "stopped") << endl;
	}
};

class ControlLevers 
{
	bool getInCar;
	bool ignition;
	unsigned short engineSpeed;
public:
	bool get_getInCar()const
	{return this->getInCar;}
	void set_ignition(bool ignition)
	{
		if (ignition) set_engineSpeed(1500);
		else set_engineSpeed(0);
	}
	unsigned short get_engineSpeed()const
	{return this->engineSpeed;}
	void  set_engineSpeed(unsigned short  engineSpeed)
	{
		if (engineSpeed < 1500 && ignition)
		{ 
			for (int i = 0; i < 100; printf("Äûð "), i++); 
			printf("*ÃËÎÕÍÅÒ*"); Sleep(800); 
			this->ignition = false; this->engineSpeed = 0;
		}
		else if (engineSpeed > MAX_GEAR_SHIFT)this->engineSpeed = 8000;
		else this->engineSpeed = engineSpeed;
	}

	ControlLevers()
	{
		this->getInCar = true;
		this->engineSpeed = 0;
		this->ignition = false;
	}
	~ControlLevers(){}

	void keyBoardHit(Engine& CEngine, Tank& CTank)
	{
		if (_kbhit())
		{
			char key;
			key = _getch();
			if (!CEngine.get_EngineWork())key = 'f';
			switch (key)
			{
			case 'f':case'F':if (CEngine.get_EngineWork());
					else { this->ignition = true; CEngine.start(); set_engineSpeed(1500); } break;
			case 'w':case 'W': set_engineSpeed(this->engineSpeed + 100); break;
			case 's':case 'S': set_engineSpeed(this->engineSpeed - 100);if(engineSpeed<1500)CEngine.stop(); break;
			case 'q':case 'Q': this->ignition = false; CEngine.stop(); set_engineSpeed(0); break;
			case 27: this->getInCar = false;
			}
		}
	}

	void info()
	{
		printf("Engine Speed: %u\n", engineSpeed);
		//for (int i = 0; i < (engineSpeed - 1500) / 100; cout << "|", i++);
		//cout << endl;
	}
};

class Car 
{
	Engine CEngine;
	Tank CTank;
	ControlLevers CControlLevels;
public:
	Engine get_Cengine()const
	{return CEngine;}
	Tank get_CTank()const
	{return CTank;}
	ControlLevers get_CControlLevels()const
	{return CControlLevels;}

	Car(double fuel_level, double consumption)
	{
		this->CTank.fill(fuel_level);
		this->CEngine.set_consumption(consumption);
	}
	~Car(){}

	void CarOn()
	{
		while (CControlLevels.get_getInCar())
		{
			if (CTank.get_IsComplete()) { CControlLevels.set_ignition(false); CEngine.stop(); }
			if (CEngine.get_EngineWork())
			{
				CTank.give_fuel(CEngine.get_consumption_fer_second()+CEngine.ConsumptionWithSpeed(CControlLevels.get_engineSpeed()));
			}
			CEngine.info(CEngine.ConsumptionWithSpeed(CControlLevels.get_engineSpeed()));
			CTank.info();
			CControlLevels.info();
			CControlLevels.keyBoardHit(CEngine, CTank);
			Sleep(50);
			system("cls");
		}
	}
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
		cout << "Ââåäèòå îáúåì: "; cin >> fuel;
		tank.fill(fuel);
		tank.info();
	}
#endif // TANK_CH

#ifdef ENGINE_CH
	Engine engine(9);
	engine.info();
#endif // ENGINE_CH

#ifdef CAR_CH
	Car Car1(60, 8);
	Car1.CarOn();
#endif // CAR_CH

}