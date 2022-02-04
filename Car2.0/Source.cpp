#include<iostream>
#include<thread>
#include<conio.h>
#include<Windows.h>
#include<chrono>
#include<ctime>
#include <queue>
using namespace std;
using std::chrono::system_clock;
using std::cout;
using std::cin;
using std::endl;

#pragma warning(disable : 4996)

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

#define MAX_SPEED_LOW 120
#define MAX_SPEED_HIGH 400

class Car
{
	Tank tank;
	Engine engine;
	bool driver_inside;
	time_t timeNow;
	time_t timeStartInsideCar;
	double last_Engine_Work;
	struct Control
	{
		thread panel_thread;
		thread engine_idle_thread;
		thread free_wheeling_thread;
		thread time_thred;
	}control;
	const int MAX_SPEED;
	int speed;
	double trip;
	double consumption_by_time;
public:
	Car(double engine_consumption, unsigned int tank_volume, int max_speed, time_t timeNow = 0, 
		time_t timeInsideCar = 0, double last_Engine_Work = 0, double trip = 0.0, double consumption_by_time = 0)
		:engine(engine_consumption),
		tank(tank_volume),
		MAX_SPEED(max_speed >= MAX_SPEED_LOW && max_speed <= MAX_SPEED_HIGH ? max_speed : 200),
		timeNow(timeNow),
		timeStartInsideCar(timeInsideCar),
		last_Engine_Work(last_Engine_Work),
		trip(trip),
		consumption_by_time(consumption_by_time)
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
			system_clock::time_point today = system_clock::now();
			timeStartInsideCar = system_clock::to_time_t(today);
		}
	}
	void stop_engine()
	{
		engine.stop();
		if(control.engine_idle_thread.joinable())control.engine_idle_thread.join();
		last_Engine_Work = difftime(timeNow, timeStartInsideCar)+ last_Engine_Work;
	}
	void get_in()
	{
		driver_inside = true;
		control.panel_thread = std::thread(&Car::control_panel, this);
		control.time_thred = std::thread(&Car::time_panel, this);
	}
	void get_out()
	{
		last_Engine_Work = 0;
		trip = 0;
		consumption_by_time = 0;
		driver_inside = false;
		if(control.panel_thread.joinable())control.panel_thread.join();
		if (control.time_thred.joinable())control.time_thred.join();
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
			case Enter: if (speed > 0) { cout << "Сначала остановитесь!" << endl; break; }if (driver_inside)get_out(); else get_in(); break;
			case 'f':case 'F':if (driver_inside) { cout << "Сначала выйдите из машины!" << endl; break; }
					double fuel; cout << "Введите объем топлива: "; cin >> fuel; fill(fuel); break;
			case 'i': case 'I':if (engine.get_EngineWork())stop_engine(); else start_engine(); break;
			case 'w':case 'W':if (engine.get_EngineWork()&& speed<=MAX_SPEED)speed += 10;
				if (!control.free_wheeling_thread.joinable())
				{
					control.free_wheeling_thread = std::thread(&Car::free_wheeling, this);
				}
				std::this_thread::sleep_for(600ms);
				break;
			case 's':case 'S':if (speed > 0)speed -= 10; if (speed < 0)speed = 0; std::this_thread::sleep_for(400ms); break;
			case Escape: stop_engine(); get_out();if (control.free_wheeling_thread.joinable()) { speed = 0; control.free_wheeling_thread.join(); } break;
			}
			if (control.free_wheeling_thread.joinable() && speed == 0)control.free_wheeling_thread.join();
		} while (key != 27);
	}

	void engine_idle()
	{
		while (engine.get_EngineWork() && tank.give_fuel(engine.get_consumption_per_second() + consumptionOfSpeed())) 
		{
			consumption_by_time += engine.get_consumption_per_second() + consumptionOfSpeed();
			std::this_thread::sleep_for(1s);
		}
	}
	void free_wheeling()
	{
		while (speed)
		{
			trip += double(speed)/3600;
			speed--;
			if (speed < 0)speed = 0;
			std::this_thread::sleep_for(1s);
		}
	}

	void control_panel()
	{
		double timedif = 0.0;
		while (driver_inside)
		{
			system("CLS");
			cout << "Real time is: " << ctime(&timeNow);
			if(engine.get_EngineWork())timedif = difftime(timeNow, timeStartInsideCar);
			cout << "Time from start engine: " << timedif+last_Engine_Work << " sec;" << endl;
			for (int i = 0; i < speed / 3; i++)
			{ 
				HANDLE hConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
				if(i>150/3)SetConsoleTextAttribute(hConsoleHandle, 0x0E);
				if(i>200/3)SetConsoleTextAttribute(hConsoleHandle, 0x0C);
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
			cout << "Consumption per second: " << engine.get_consumption_per_second()+consumptionOfSpeed() << endl;
			printf("Consumption per all trip: %2.4f\n", consumption_by_time);
			if(engine.get_EngineWork())printf("Middle consumption/s: %1.4f\n",(consumption_by_time /(timedif+last_Engine_Work)));
			cout << "Enhgine is: " << (engine.get_EngineWork() ? "started" : "stoped") << endl;
			printf("Trip distance : %3.2f km\n", trip);
			std::this_thread::sleep_for(100ms);
		}
	}

	void time_panel()
	{
		while (driver_inside)
		{
			system_clock::time_point today = system_clock::now();
			timeNow = system_clock::to_time_t(today);
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
	Car bmw(40, 80 ,250);
	bmw.control_car();
#endif // CAR_CH

}