#include "stdafx.h"

#include "../../Src/WaitableTimer.h"
#include "../../Src/StopWatch.h"

const int NumIterations = 50;
const int Period = 250;

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
		printf("Elapsed time: %I64d milliseconds (%dx%d = %d)\n", ElapsedTime,
			NumIterations, Period, NumIterations*Period);
	}
	catch(std::exception& ex)
	{
		fprintf(stderr, "Error: %s\n", ex.what());
	}
	return 0;
}

