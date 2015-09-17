#include "stdafx.h"

#include <vector>
#include "../../Src/Thread.h"

class PrintNumber: public AC::Runnable
{
protected:
	int m_Number;

public:
	PrintNumber(int Number) : m_Number(Number) {}

	void Run()
	{
		printf("Number: %d\n", m_Number);
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	const int numThreads = 10;
	try
	{
		for (int i = 0; i < numThreads; i++)
		{
			AC::Thread Thread(new PrintNumber(i));
			Thread.Start();
			Thread.Wait();
		}

		std::vector<AC::Thread> Threads;
		for (int i = 0; i < numThreads; i++)
		{
			AC::Thread Thread(new PrintNumber(i));
			Thread.Start();
			Threads.push_back(Thread);
		}
		for (unsigned i = 0; i < Threads.size(); i++)
			Threads[i].Wait();
	}
	catch(std::exception& ex)
	{
		fprintf(stderr, "Exception: %s\n", ex.what());
	}

	return 0;
}