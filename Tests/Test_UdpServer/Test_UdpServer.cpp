// Test_UdpServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <list>
#include <algorithm>

#include "../../Src/Socket.h"
#include "../../Src/ScopedWinSock.h"
#include "../../Src/UdpClient.h"
#include "../../Src/UdpServer.h"
#include "../../Src/Thread.h"
#include "../../Src/WaitableTimer.h"

#pragma comment(lib, "WS2_32.LIB")

const int NUM_CONNECTIONS = 100;

class Server: public AC::Runnable
{
protected:
	std::list<AC::InetAddress> m_Clients;
	AC::UdpServer m_Server;
	AC::WaitableTimer m_Timer;
	AC::Event m_SocketEvent;
	AC::Event& m_StopEvent;
	std::vector<HANDLE> m_SyncVector;

	void HandleReadEvent()
	{
		int Data;
		AC::InetAddress From;
		std::list<AC::InetAddress>::iterator iter;
		WSANETWORKEVENTS NetworkEvents;

		try
		{
			m_Server.EnumNetworkEvents(m_SocketEvent, &NetworkEvents);

			m_Server.RecvFrom(Data, From);
			iter = std::find(m_Clients.begin(), m_Clients.end(), From);
			if (iter == m_Clients.end())
			{
				_tprintf(_T("Client connected from %s\n"), From.Str().c_str());
				m_Clients.push_back(From);
			}
			else
			{
				_tprintf(_T("\tMessage from %s\n"), From.Str().c_str());
				assert(Data == 32);
			}
		}
		catch(...)
		{
			_tprintf(_T("Client disconnected  from %s\n"), From.Str().c_str());
			m_Clients.remove(From);
		}
	}

	void HandleTimerEvent()
	{
		std::list<AC::InetAddress>::iterator iter;
		int Data = 33;

		for (iter = m_Clients.begin(); iter != m_Clients.end(); iter++)
			m_Server.SendTo(Data, *iter);
	}

public:
	Server(AC::InetAddress Addr, DWORD Period, AC::Event StopEvent)
		:m_Server(Addr), m_StopEvent(StopEvent)
	{
		m_Timer.StartMilliseconds(Period);
		m_Server.EventSelect(m_SocketEvent, FD_READ);

		m_SyncVector.push_back(m_SocketEvent);
		m_SyncVector.push_back(m_Timer);
		m_SyncVector.push_back(m_StopEvent);
	}

	void Run()
	{
		try
		{
			const int Timeout = 10000;
			for (;;)
			{
				int ret = WaitForMultipleObjects(m_SyncVector.size(), &m_SyncVector[0], FALSE, INFINITE);
				if (ret == WAIT_OBJECT_0)
					HandleReadEvent();
				else if (ret == WAIT_OBJECT_0 + 1)
					HandleTimerEvent();
				else if (ret == WAIT_OBJECT_0 + 2)
					break;
				else
					assert(false);
			}
		}
		catch(std::exception& ex)
		{
			fprintf(stderr, "[Server thread] Error: %s\n", ex.what());
		}
	}
};

class Client: public AC::Runnable
{
protected:
	AC::InetAddress m_ServerAddr;

public:
	Client(AC::InetAddress Addr)
		:m_ServerAddr(Addr)
	{
	}

public:
	void Run()
	{
		try
		{
			AC::UdpClient Client;
			AC::InetAddress Addr;

			Client.SendTo(32, m_ServerAddr);

			int Data;
			Client.RecvFrom(Data, Addr);
			assert(Data == 33);
		}
		catch(std::exception& ex)
		{
			fprintf(stderr, "[Client thread] Error: %s\n", ex.what());
		}
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);

	try
	{
		AC::ScopedWinSock UsingWinSock;
		AC::Event StopEvent;
		AC::InetAddress Addr(_T("localhost"), 9876);
		AC::Thread ThreadServer(new Server(Addr, 1000, StopEvent));
		ThreadServer.Start();

		for (int i = 0; i < NUM_CONNECTIONS; i++)
		{
			AC::Thread ThreadClient(new Client(Addr));
			ThreadClient.Start();
		}

		Sleep(5000);
		StopEvent.SetEvent();
		ThreadServer.Wait();
	}
	catch(std::exception& ex)
	{
		fprintf(stderr, "Error: %s\n", ex.what());
	}

	return 0;
}

