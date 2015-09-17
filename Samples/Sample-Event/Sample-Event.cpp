#include "stdafx.h"

#include "../../Src/Event.h"
#include "../../Src/Thread.h"

#include <time.h>

const unsigned short NUM_THREADS = 5;
const unsigned long DELAY_MS = 2000;

class TaskPrint : public AC::Runnable
{
private:
	TaskPrint(const TaskPrint& objectSrc);		
	void operator=(const TaskPrint& objectSrc);	

protected:
	AC::Event& m_Event;
	unsigned short m_Hilo;

public:
	TaskPrint(AC::Event& ev, unsigned short n) 
		: m_Event(ev), m_Hilo(n) {}

	void Run()
	{
		try
		{
			m_Event.Wait();			
			printf("Hilo %d ejecutando tarea\n", m_Hilo);
			Sleep(DELAY_MS*m_Hilo);
		}
		catch(std::exception& ex)
		{
			fprintf(stderr, "Exception: %s\n", ex.what());
		}			
	}

};

int _tmain(int argc, _TCHAR* argv[])
{
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);

	try
	{
		AC::Event Event(FALSE, TRUE);
		std::vector<AC::Thread> Threads;
		for (unsigned short i = 1; i <= NUM_THREADS; i++)
		{
			AC::Thread Thread(new TaskPrint(Event, i));
			Thread.Start();
			Threads.push_back(Thread);
		}
		Sleep(DELAY_MS);
		Event.SetEvent();
		for (unsigned short i = 0; i < Threads.size(); i++)
			Threads[i].Wait();
	}
	catch(std::exception& ex)
	{
		fprintf(stderr, "Exception: %s\n", ex.what());
	}

	return 0;
}

