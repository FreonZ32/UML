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
	double default_consumption;
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
		this->default_consumption = this->consumption;
		consumption_per_second = this->consumption * .3e-4;
	}
	void set_EngineWork(bool EngineWork)
	{
		this->EngineWork = EngineWork;
	}
	explicit Engine(double consumption)
	{
		default_consumption = this->consumption;
		set_consumption(consumption);
		set_EngineWork(false);
		printf("Engine is ready: %p\n", this);
	}
	~Engine()
	{
		printf("Engine is gone! \n");
	}
	double set_consumption_by_speed(int speed)
	{
		set_consumption(get_consumption());
		if (speed > 0 && speed < 60)consumption_per_second *= 6.6;
		else if (speed > 60 && speed <= 100)consumption_per_second*= 4.6;
		else if (speed > 100 && speed <= 140)consumption_per_second*= 6.6;
		else if (speed > 140 && speed <= 200)consumption_per_second*= 8.3;
		else if (speed > 200 && speed < 300)consumption_per_second*= 10;
		return consumption_per_second;
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

#define MAX_SPEED_LOW 120
#define MAX_SPEED_HIGH 400

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
	const int MAX_SPEED;
	int speed;
public:
	Car(double engine_consumption, unsigned int tank_volume, int max_speed)
		:engine(engine_consumption),
		tank(tank_volume),
		MAX_SPEED(max_speed >= MAX_SPEED_LOW && max_speed <= MAX_SPEED_HIGH ? max_speed : 200)
	{
		driver_inside = false;
		speed = 0;
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
		if (tank.get_fuel_level())
		{
			engine.start();
			control.engine_idle_thread = std::thread(&Car::engine_idle, this);
		}
	}
	void stop_engine()
	{
		engine.stop();
		if (control.engine_idle_thread.joinable())control.engine_idle_thread.join();
	}
	void get_in()
	{
		driver_inside = true;
		control.panel_thread = std::thread(&Car::control_panel, this);
	}
	void get_out()
	{
		driver_inside = false;
		if (control.panel_thread.joinable())control.panel_thread.join();
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
			case 'w':case 'W':if (engine.get_EngineWork() && speed <= MAX_SPEED)speed += 10;
				if (!control.free_wheeling_thread.joinable())
				{
					control.free_wheeling_thread = std::thread(&Car::free_wheeling, this);
				}
				std::this_thread::sleep_for(600ms);
				break;
			case 's':case 'S':if (speed > 0)speed -= 10; if (speed < 0)speed = 0; std::this_thread::sleep_for(400ms); break;
			case Escape: stop_engine(); get_out(); if (control.free_wheeling_thread.joinable()) { speed = 0; control.free_wheeling_thread.join(); } break;
			}
			if (control.free_wheeling_thread.joinable() && speed == 0)control.free_wheeling_thread.join();
			engine.set_consumption_by_speed(speed);
		} while (key != 27);
	}

	void engine_idle()
	{
		while (engine.get_EngineWork() && tank.give_fuel(engine.get_consumption_per_second()))
			std::this_thread::sleep_for(1s);
	}
	void free_wheeling()
	{
		while (speed)
		{
			speed--;
			if (speed < 0)speed = 0;
			std::this_thread::sleep_for(1s);
		}
	}

	void control_panel()
	{
		while (driver_inside)
		{
			system("CLS");
			for (int i = 0; i < speed / 3; i++)
			{
				HANDLE hConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
				if (i > 150 / 3)SetConsoleTextAttribute(hConsoleHandle, 0x0E);
				if (i > 200 / 3)SetConsoleTextAttribute(hConsoleHandle, 0x0C);
				cout << "|";
				SetConsoleTextAttribute(hConsoleHandle, 7);
			}cout << endl;
			cout << "Fuel level: " << tank.get_fuel_level() << " liters." << endl;
			if (tank.get_fuel_level() < 5)
			{
				HANDLE hConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleTextAttribute(hConsoleHandle, 4);
				cout << "Low fuel" << endl;
				SetConsoleTextAttribute(hConsoleHandle, 7);
			}
			cout << "Speed if vehicle: " << speed << endl;
			cout << "Consumption per second: " << engine.get_consumption_per_second() << endl;
			cout << "Enhgine is: " << (engine.get_EngineWork() ? "started" : "stoped") << endl;
			std::this_thread::sleep_for(200ms);
		}
	}

	double consumptionOfSpeed()
	{
		if (speed < 60)return 0;
		else if (speed >= 60 && speed < 150)return -(engine.get_consumption_per_second() / 4);
		else if (speed >= 150 && speed < 200)return engine.get_consumption_per_second() / 4;
		else if (speed >= 200 && speed < MAX_SPEED_HIGH)return engine.get_consumption_per_second() / 3;
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
	Car bmw(40, 80, 250);
	bmw.control_car();
#endif // CAR_CH

}