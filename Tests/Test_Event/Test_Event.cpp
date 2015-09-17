// Test_Event.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

#include <vector>

#include "../../Src/Event.h"
#include "../../Src/Thread.h"

class WaitEvent: public AC::Runnable
{
protected:
	AC::Event& m_Event;

public:
	WaitEvent(AC::Event& ev) : m_Event(ev) {}

	void Run()
	{
		m_Event.Wait();
		printf("Hello\n");
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		AC::Event Event(FALSE, TRUE);

		AC::Thread Thread1(new WaitEvent(Event));
		Thread1.Start();

		AC::Thread Thread2(new WaitEvent(Event));
		Thread2.Start();

		Sleep(2000);

		Event.SetEvent();

		Thread1.Wait();
		Thread2.Wait();

		std::vector<HANDLE> Objects;
		Objects.push_back(Thread1);
		Objects.push_back(Thread2);
		Objects.push_back(Event);

		int ret = WaitForMultipleObjects(Objects.size(), &Objects[0], TRUE, INFINITE);
		assert(ret == WAIT_OBJECT_0);
	}
	catch(std::exception& ex)
	{
		fprintf(stderr, "Exception: %s\n", ex.what());
	}

	return 0;
}

