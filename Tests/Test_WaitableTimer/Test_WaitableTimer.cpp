// Test_WaitableTimer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../../Src/WaitableTimer.h"
#include "../../Src/StopWatch.h"

const int NumIterations = 50;
const int Period = 250;
const int MaxError = 100;

#define ABS(a)	   (((a) < 0) ? -(a) : (a))

int _tmain(int argc, _TCHAR* argv[])
{
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);

	try
	{
		AC::WaitableTimer WaitableTimer;

		AC::StopWatch StopWatch;

		WaitableTimer.StartMilliseconds(Period);
		for (int i = 0; i < NumIterations; i++)
			WaitableTimer.Wait();

		__int64 ElapsedTime = StopWatch.ElapsedMilliseconds();
		printf("Test took: %I64d milliseconds (%dx%d = %d)\n", ElapsedTime,
			NumIterations, Period, NumIterations*Period);

		assert(ABS(ElapsedTime - NumIterations*Period) < MaxError);
	}
	catch(std::exception& ex)
	{
		fprintf(stderr, "Exception: %s\n", ex.what());
	}
	return 0;
}

