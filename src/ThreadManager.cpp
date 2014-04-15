#include "ThreadManager.h"


ThreadManager::ThreadManager(void)
{
	testVar=1;
	testVar2=2;
	testVar3=0;
	testVar4=0;
}


ThreadManager::~ThreadManager(void)
{
}


void ThreadManager::registerThread()
{	
	
	threads.push_back(std::thread());
}

void ThreadManager::testMethod()
{
	cout<<"METHOD 1\n"<<"--------------"<<endl;
	testVar3+=testVar*testVar2;
	cout<<"Total: "<<testVar3<<endl;
}

void ThreadManager::testMethod2(int val)
{
	cout<<"METHOD 2\n"<<"--------------"<<endl;
	testVar4+=testVar+testVar2;
	cout<<"Total: "<<testVar4<<endl;
}

void ThreadManager::test()
{
	
	
}
