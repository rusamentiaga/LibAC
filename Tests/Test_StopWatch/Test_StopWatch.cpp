// Test_StopWatch.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../../Src/StopWatch.h"


int _tmain(int argc, _TCHAR* argv[])
{
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);

	try
	{
		AC::StopWatch StopWatch;

		StopWatch.Start();
		printf("Testing printf...\n");
		printf("printf took: %I64d microseconds\n", StopWatch.ElapsedMicroseconds());

		StopWatch.Start();
		std::cout << "Testing cout..." << std::endl;
		printf("cout took: %I64d microseconds\n", StopWatch.ElapsedMicroseconds());
	}
	catch(std::exception& ex)
	{
		fprintf(stderr, "Exception: %s\n", ex.what());
	}

	return 0;
}

