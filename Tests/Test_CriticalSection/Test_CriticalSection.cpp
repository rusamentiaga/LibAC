// Test_CriticalSection.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <assert.h>
#include <windows.h>
#include "../../Src/CriticalSection.h"
#include "../../Src/ScopedCriticalSection.h"

int g_Number;
AC::CriticalSection g_csNumber;

const int NUM_ITERATIONS = 1000000;

DWORD WINAPI ThreadProcNotSafe(LPVOID lpParam) 
{ 
	UNREFERENCED_PARAMETER(lpParam);

	for (int i = 0; i < NUM_ITERATIONS; i++)
	{
		g_Number++;
		g_Number--;
	}

	return 0;
}

DWORD WINAPI ThreadProcSafe(LPVOID lpParam) 
{ 
	UNREFERENCED_PARAMETER(lpParam);

	for (int i = 0; i < NUM_ITERATIONS; i++)
	{
		g_csNumber.Enter();
		g_Number++;
		g_Number--;
		g_csNumber.Leave();
	}

	return 0;
}

DWORD WINAPI ThreadProcAuto(LPVOID lpParam) 
{ 
	UNREFERENCED_PARAMETER(lpParam);

	for (int i = 0; i < NUM_ITERATIONS; i++)
	{
		AC::ScopedCriticalSection Lock(g_csNumber);
		g_Number++;
		g_Number--;
	}

	return 0;
}

void Test(LPTHREAD_START_ROUTINE lpStartAddress)
{
	const int NUM_THREADS = 10;
	HANDLE hThreads[NUM_THREADS];

	g_Number = 0;

	for (int i = 0; i < NUM_THREADS; i++)
	{
		hThreads[i] = CreateThread(NULL, 0, lpStartAddress, NULL, 0, NULL);
		if (hThreads[i] == NULL)
		{
			fprintf(stderr, "Error creating thread: %d\n", GetLastError());
			return;
		}
	}
	for (int i = 0; i < NUM_THREADS; i++)
		WaitForSingleObject(hThreads[i], INFINITE);

	for (int i = 0; i < NUM_THREADS; i++)
		CloseHandle(hThreads[i]);
}

int _tmain(int argc, _TCHAR* argv[])
{
	Test(ThreadProcNotSafe);
	printf("ThreadProcNotSafe g_Number = %d\n", g_Number);

	Test(ThreadProcSafe);
	printf("ThreadProcSafe g_Number = %d\n", g_Number);
	assert(g_Number == 0);

	Test(ThreadProcAuto);
	printf("ThreadProcAuto g_Number = %d\n", g_Number);
	assert(g_Number == 0);

	return 0;
}

