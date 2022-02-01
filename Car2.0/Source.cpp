#include<iostream>
#include<thread>
#include<conio.h>
#include<Windows.h>
#include<chrono>
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
	{
		return VOLUME;
	}
	double get_fuel_level()const
	{
		return fuel_level;
	}

	Tank(unsigned int volume) :VOLUME(volume >= MIN_TANK_VOLUME && volume <= MAX_TANK_VOLUME ? volume : MAX_TANK_VOLUME), fuel_level(0)
	{
		printf("Tank is ready: %p\n", this);
	}
	~Tank()
	{
		printf("Tank is gone\n");
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
		printf("Tank volume: %u\n", VOLUME);
		printf("Fuel level: %4.5g\n", fuel_level);
	}
};

class Engine
{
	double consumption;
	double consumption_per_second;
	bool EngineWork;

public:
	double get_consumption()const
	{
		return consumption;
	}
	double get_consumption_per_second()const
	{
		return consumption_per_second;
	}
	bool get_EngineWork()const
	{
		return EngineWork;
	}
	void set_consumption(double consumption)
	{
		if (consumption >= MIN_ENGINE_CONSUMPTION && consumption <= MAX_ENGINE_CONSUMPTION)
			this->consumption = consumption;
		else
			this->consumption = MAX_ENGINE_CONSUMPTION / 2;
		consumption_per_second = this->consumption * .3e-4;
	}
	void set_EngineWork(bool EngineWork)
	{
		this->EngineWork = EngineWork;
	}

	explicit Engine(double consumption)
	{
		set_consumption(consumption);
		set_EngineWork(false);
		printf("Engine is ready: %p\n", this);
	}
	~Engine()
	{
		printf("Engine is gone! \n");
	}

	bool start()
	{
		EngineWork = true;
		return EngineWork;
	}
	bool stop()
	{
		EngineWork = false;
		return EngineWork;
	}

	void info()const
	{
		cout << "Consumption:  " << consumption << endl;
		cout << "Consumption per second:  " << consumption_per_second << endl;
		cout << "Engine is:  " << (EngineWork ? "started" : "stopped") << endl;
	}
};

#define Enter 13
#define Escape 27

class Car
{
	Tank tank;
	Engine engine;
	bool driver_inside;
	struct Control
	{
		thread panel_thread;
		thread engine_idle_thread;
		thread free_wheeling_thread;
	}control;
public:
	Car(double engine_consumption, unsigned int tank_volume) :engine(engine_consumption), tank(tank_volume)
	{
		driver_inside = false;
		printf("Your car is ready! %p\n", this);
	}
	~Car()
	{
		printf("Your car is over!\n");
	}

	void fill(double fuel)
	{
		tank.fill(fuel);
	}
	void start_engine()
	{
		if(tank.get_fuel_level())engine.start();
		control.engine_idle_thread = std::thread(&Car::engine_idle, this);
	}
	void stop_engine()
	{
		engine.stop();
		control.engine_idle_thread.join();
	}
	void get_in()
	{
		driver_inside = true;
		control.panel_thread = std::thread(&Car::control_panel, this);
	}
	void get_out()
	{
		driver_inside = false;
		control.panel_thread.join();
		system("CLS");
		cout << "You are out of car" << endl;
	}

	void control_car()
	{
		char key;
		do
		{
			key = _getch();
			switch (key)
			{
			case Enter: if (driver_inside)get_out(); else get_in(); break;
			case 'f':case 'F':double fuel; cout << "Введите объем топлива: "; cin >> fuel; fill(fuel); break;
			case 'i': case 'I':if (engine.get_EngineWork())stop_engine(); else start_engine(); break;
			case Escape:if (control.panel_thread.joinable())get_out(); stop_engine(); break;
			}
		} while (key != 27);
	}

	void engine_idle()
	{
		while (engine.get_EngineWork() && tank.give_fuel(engine.get_consumption_per_second()))
			std::this_thread::sleep_for(1s);
	}

	void control_panel()
	{
		while (driver_inside)
		{
			system("CLS");
			cout << "Fuel level: " << tank.get_fuel_level() << " liters." << endl;
			cout << "Enhgine is: " << (engine.get_EngineWork() ? "started" : "stoped") << endl;
			std::this_thread::sleep_for(1s);
		}
	}

	void info()const
	{
		engine.info();
		tank.info();
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
	Car bmw(0, 80);
	bmw.control_car();
#endif // CAR_CH

}