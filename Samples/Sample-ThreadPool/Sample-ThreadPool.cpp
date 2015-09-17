#include "stdafx.h"

#include <time.h>
#include "../../Src/ThreadPool.h"

const int NUM_THREADS = 2;
const int QUEUESIZE = 4;
const unsigned short NUM_TASKS = 10;

class TaskMul: public AC::Runnable
{
protected:
	int m_a;
	int m_b;
	int m_NumTask;

public:
	TaskMul(int a, int b, int n) : m_a(a), m_b(b), m_NumTask(n) {}

	void Run()
	{
		printf("[Tarea %d]: %d x %d = %d\n", m_NumTask, m_a, m_b, m_a*m_b);
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);

	try
	{
		AC::ThreadPool Pool(NUM_THREADS, QUEUESIZE);
		srand((unsigned int)time(NULL));

		for (unsigned short i = 0; i < NUM_TASKS; i++)
			Pool.AddTask(new TaskMul((int)rand(), (int)rand(), i+1));
	}
	catch(std::exception& ex)
	{
		fprintf(stderr, "Exception: %s\n", ex.what());
	}

	return 0;
}

