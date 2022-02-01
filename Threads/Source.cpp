#include<iostream>
#include<thread>
#include<chrono>
#include<Windows.h>
//using namespace std;
using namespace std::chrono;
bool finish = false;
bool flag = true;
bool flag2 = false;


void plus()
{
	while (!finish)
	{ 
		if(flag == flag2)
		{
		std::cout << "+ ";
		std::this_thread::sleep_for(10ms);
		flag2 = !flag2;
		}
	}
}

void minus()
{
	while (!finish)
	{

		if (flag == flag2)
		{
			std::cout << "- ";
			std::this_thread::sleep_for(10ms);
		}
	}
}

void main()
{
	std::thread plus_thread(plus);
	std::thread minus_thread(minus);
	
	std::cin.get();
	finish = true;
	plus_thread.join();
	minus_thread.join();
}