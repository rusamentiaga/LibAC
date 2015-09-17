#include "stdafx.h"

#include "../../Src/Semaphore.h"
#include "../../Src/Thread.h"
#include "../../Src/Exception.h"

#include <time.h>
#include <vector>

const long NUM_TICKETS = 10;
const long NUM_SELLERS = 2;
const unsigned long DELAY_MS = 1000;

class Seller : public AC::Runnable
{
private:
	Seller(const Seller& objectSrc);		
	void operator=(const Seller& objectSrc);		

protected:
	AC::Semaphore& m_Sem;
	unsigned short m_Id;

public:
	Seller(AC::Semaphore& s, unsigned short n) 
		: m_Sem(s), m_Id(n) {}

	void Run()
	{
		unsigned int SoldByMe = 0;
		try
		{
			while(m_Sem.Wait(0) != WAIT_TIMEOUT)
			{
				Sleep(DELAY_MS*m_Id);	// Simula el tiempo consumido en la venta
				SoldByMe++;
				printf("Vendedor %d: vendo un ticket\n", m_Id);		
			}
			printf("Vendedor %d: todos vendidos, he vendido %d tickets\n", m_Id, SoldByMe);
		}
		catch(std::exception& ex)
		{
			fprintf(stderr, "Exception: %s\n", ex.what());
			return;
		}		
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);
    
	try
	{
		AC::Semaphore Tickets(NUM_TICKETS, NUM_TICKETS);
		std::vector<AC::Thread> Threads;
		for (unsigned short i = 0; i < NUM_SELLERS; i++)
		{
			AC::Thread Thread(new Seller(Tickets, i+1));
			Thread.Start();
			Threads.push_back(Thread);
		}
		for (unsigned short i = 0; i < Threads.size(); i++)
			Threads[i].Wait();
	}
	catch(std::exception& ex)
	{
		fprintf(stderr, "Exception: %s\n", ex.what());
	}

	return 0;
}

