#include "stdafx.h"

#include "../../Src/CriticalSection.h"
#include "../../Src/ScopedCriticalSection.h"
#include "../../Src/Thread.h"

class SharedData
{
protected:
	int	m_x;
	AC::CriticalSection m_cs;

public:
	SharedData() 
		: m_x(0) {}

	void Inc() 
	{ 
		AC::ScopedCriticalSection ScopedCritSec(m_cs);
		m_x++; 
	}
	void Dec()
	{
		AC::ScopedCriticalSection ScopedCritSec(m_cs);
		m_x--; 
	}
	int Get() 
	{ 
		int x;
		{
			AC::ScopedCriticalSection ScopedCritSec(m_cs);
			x = m_x; 
		}
		return x;
	}	
};	

class DataAccessTask : public AC::Runnable
{
private:
	DataAccessTask(const DataAccessTask& objectSrc);
	void operator=(const DataAccessTask& objectSrc);

protected:
	SharedData& m_Data;

public:
	DataAccessTask(SharedData& d) : m_Data(d) {}

	void Run()
	{
		m_Data.Inc();
		m_Data.Dec();
		printf("El valor de la variable compartida es %d\n", m_Data.Get());
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);

	try
	{
		SharedData Data;

		AC::Thread AccessThread1(new DataAccessTask(Data));
		AC::Thread AccessThread2(new DataAccessTask(Data));
		AccessThread1.Start();
		AccessThread2.Start();
		
		AccessThread1.Wait();
		AccessThread2.Wait();
	}
	catch(std::exception& ex)
	{
		fprintf(stderr, "Error: %s\n", ex.what());
	}

	return 0;
}

