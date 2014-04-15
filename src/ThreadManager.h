#pragma once

#include <string>
#include <iostream>
#include <stdarg.h>
#include <thread>
#include <list>

using namespace std;

class ThreadManager
{
public:
	ThreadManager(void);
	~ThreadManager(void);
	
	//methods
	void testMethod();
	void testMethod2(int val);
	void test();
	void registerThread();

	//variables
	std::list<thread> threads;
	int testVar;
	int testVar2;
	int testVar3;
	int testVar4;
};

