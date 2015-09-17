#include "stdafx.h"

#include <math.h>
#include <time.h>

#include "../../Src/SharedQueue.h"
#include "../../Src/Thread.h"

const unsigned short NUM_POLYNOMIALS = 50;

typedef std::pair<double, double> Pair;

class MathTask : public AC::Runnable
{
private:
	MathTask(const MathTask& objectSrc);		
	void operator=(const MathTask& objectSrc);	

protected:
	AC::SharedQueue<Pair>& m_InputQueue;
	AC::SharedQueue<Pair>& m_OutputQueue;

	double Compute(double x)
	{
		double y = 0.5*pow(x,2) + 1.25*x + 127; // y = 0.5x^2+1.25x+127
		return y;
	}

public:
	MathTask(AC::SharedQueue<Pair>& InputQueue, AC::SharedQueue<Pair>& OutputQueue)
		: m_InputQueue(InputQueue), m_OutputQueue(OutputQueue) {}

	void Run()
	{
		try
		{
			for (unsigned short i = 0; i < NUM_POLYNOMIALS; i++)
			{
				Pair p = m_InputQueue.Pop();			
				p.second = Compute(p.first);
				m_OutputQueue.Push(p);
			}
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

	srand((unsigned int)time(NULL));

	try
	{
		AC::SharedQueue<Pair> InputQueue(NUM_POLYNOMIALS), OutputQueue(NUM_POLYNOMIALS);
		AC::Thread Thread(new MathTask(InputQueue, OutputQueue));
		Thread.Start();
		
		for (unsigned short i = 0; i < NUM_POLYNOMIALS; i++)
		{
			Pair p;
			p.first = rand();
			InputQueue.Push(p);
		}
		for (unsigned short i = 0; i < NUM_POLYNOMIALS; i++)
		{
			Pair p = OutputQueue.Pop();
			printf( "El valor del polinomio para x=%f es y=%f\n", p.first, p.second);
		}		

		Thread.Wait();
	}
	catch(std::exception& ex)
	{
		fprintf(stderr, "Exception: %s\n", ex.what());
	}
	
	return 0;
}

