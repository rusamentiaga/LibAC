// Test_ThreadPool.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <assert.h>
#include "../../Src/ThreadPool.h"

int g_AliveTaskPrint = 0;

class TaskPrint: public AC::Runnable
{
protected:
	int m_TaskNum;

public:
	TaskPrint(int TaskNum) : m_TaskNum(TaskNum) 
	{
		g_AliveTaskPrint++;
	}
	void Run()
	{
		printf("Executing task %d\n", m_TaskNum);
	}

	virtual ~TaskPrint()
	{
		g_AliveTaskPrint--;
		printf("Deleting task %d\n", m_TaskNum);
	}
};

class TaskMul: public AC::Runnable
{
protected:
	int m_a;
	int m_b;

public:
	TaskMul(int a, int b) : m_a(a), m_b(b) {}
	void Run()
	{
		printf("%d x %d = %d\n", m_a, m_b, m_a*m_b);
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);

	try
	{
		AC::ThreadPool Pool(1, 20);

		for (int i = 0; i < 10; i++)
		{
			Pool.AddTask(new TaskPrint(i));
			Pool.AddTask(new TaskMul(i, i + 1));
			Pool.AddTask(new TaskMul(i*10, i*100));
		}
	}
	catch(std::exception& ex)
	{
		fprintf(stderr, "Exception: %s\n", ex.what());
	}
	assert(g_AliveTaskPrint == 0);

	return 0;
}

