// Test_SharedQueue.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../../Src/SharedQueue.h"
#include "../../Src/Thread.h"

#include <map>

const int NUM_ITER = 10000;

class DataTracker
{
protected:
	std::map<int, bool> m_Tracker;
	AC::CriticalSection m_csTracker;

public:
	void SetProduced(int item)
	{
		AC::ScopedCriticalSection Lock(m_csTracker);
		m_Tracker[item] = true;
	}

	void SetConsumed(int item)
	{
		AC::ScopedCriticalSection Lock(m_csTracker);
		assert(m_Tracker[item] == true);
		m_Tracker[item] = false;
	}

	void CheckAllConsumed()
	{
		std::map<int, bool>::iterator iter;

		for (iter = m_Tracker.begin(); iter != m_Tracker.end(); iter++)
			assert(m_Tracker[iter->second] == false);
	}
};

DataTracker g_DataTracker;

class Consumer: public AC::Runnable
{
protected:
	AC::SharedQueue<int>* m_SharedQueue;
	int m_NumIter;

public:
	Consumer(AC::SharedQueue<int>* SharedQueue, int NumIter) 
		: m_SharedQueue(SharedQueue), m_NumIter(NumIter) {}

	void Run()
	{
		for (int i = 0; i < m_NumIter; i++)
		{
			int x = m_SharedQueue->Pop();
			printf("Consumed value %d\n", x);

			g_DataTracker.SetConsumed(x);
		}
	}
};

class Producer: public AC::Runnable
{
protected:
	AC::SharedQueue<int>* m_SharedQueue;
	int m_NumIter;

public:
	Producer(AC::SharedQueue<int>* SharedQueue, int NumIter) 
		: m_SharedQueue(SharedQueue), m_NumIter(NumIter) {}

	void Run()
	{
		for (int i = 0; i < m_NumIter; i++)
		{
			g_DataTracker.SetProduced(i);

			m_SharedQueue->Push(i);

			printf("Produced value %d\n", i);
		}
	}
};


int _tmain(int argc, _TCHAR* argv[])
{
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);

	try
	{
		AC::SharedQueue<int> Queue(5);

		AC::Thread ThreadProducer(new Producer(&Queue, NUM_ITER));
		AC::Thread ThreadConsumer1(new Consumer(&Queue, NUM_ITER/2));
		AC::Thread ThreadConsumer2(new Consumer(&Queue, NUM_ITER/2));
	
		ThreadProducer.Start();
		ThreadConsumer1.Start();
		ThreadConsumer2.Start();

		ThreadProducer.Wait();
		ThreadConsumer1.Wait();
		ThreadConsumer2.Wait();

		g_DataTracker.CheckAllConsumed();
	}
	catch(std::exception& ex)
	{
		fprintf(stderr, "Error: %s\n", ex.what());
	}

	return 0;
}

