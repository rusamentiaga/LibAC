// Test_Tcp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "../../Src/ScopedWinSock.h"
#include "../../Src/Socket.h"
#include "../../Src/TcpClient.h"
#include "../../Src/TcpListener.h"
#include "../../Src/Thread.h"
#include "../../Src/StopWatch.h"

#pragma comment(lib, "WS2_32.LIB")

#define NUM_CON_STRESS 25000

class CloseableServer: public AC::Runnable
{
protected:
	AC::TcpListener Listener;

public:
	void Run()
	{
		try
		{
			Listener.Listen(3456);
			AC::TcpClient Service = Listener.Accept();
		}
		catch(std::exception&)
		{
			assert(GetLastError() == WSAEINTR); 
		}
	}

	void CloseServer()
	{
		Listener.Close();
	}
};

void DisableTimeWait(AC::TcpClient Socket)
{
	struct linger zeroLinger;
	zeroLinger.l_onoff = 1;
	zeroLinger.l_linger = 0;

	// force shutdown on socket close... avoid TIME_WAIT and FIN_WAIT states
	if(setsockopt(Socket, SOL_SOCKET, SO_LINGER, (const char *)&zeroLinger, sizeof(zeroLinger)) == SOCKET_ERROR)
		throw std::exception("Error in setsockopt");
}

void DisableNagle(AC::TcpClient Socket)
{
	int optionValue = 1;
				
	// Disable Nagle algorithm... poor performance but instant message delivery
	if(setsockopt(Socket, IPPROTO_TCP, TCP_NODELAY, (const char *)&optionValue, sizeof(optionValue)) == SOCKET_ERROR)
		throw std::exception("Error in setsockopt");
}

void ReuseAddr(AC::TcpClient Socket)
{
	int optionValue = 1;
				
	// allow socket to be bound to a port that was already in use... saves socket descriptors especially for CLIENT ... don't do for SERVER
	if(setsockopt(Socket, SOL_SOCKET, SO_REUSEADDR, (const char *)&optionValue, sizeof(optionValue)) == SOCKET_ERROR)
		throw std::exception("Error in setsockopt");
}

class StressServer: public AC::Runnable
{
protected:
	AC::TcpListener Listener;

public:
	void Run()
	{
		try
		{
			Listener.Listen(3456);
			for (int i = 0;i < NUM_CON_STRESS;i++)
			{
				AC::TcpClient Service = Listener.Accept();

				DisableTimeWait(Service);
			}
		}
		catch(std::exception& ex)
		{
			fprintf(stderr, "Error: %s\n", ex.what());
		}
	}
};


int _tmain(int argc, _TCHAR* argv[])
{
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);

	std::locale::global(std::locale(""));
	try
	{
		AC::ScopedWinSock UsingWinSock;

		AC::TcpListener Listener(9876);
		AC::TcpClient Client(AC::InetAddress(_T("localhost"), 9876));

		AC::TcpClient Service = Listener.Accept();

		// Envío de cadenas de caracteres TCHAR
		char SendName[256];
		lstrcpyA(SendName, "ruf\n");
	 	Client.SendLine(SendName);
		std::string RecvName = Service.RecvLine();
		assert(RecvName == SendName);

		// Envío de cadenas de caracteres string
		std::string s1 = "Prueba\n";
		Client.SendLine(s1);
		std::string s2 = Service.RecvLine();
		assert(s1 == s2);

		// Envío de cadenas de caracteres const char*
		Client.SendLine("Prueba\n");
		s2 = Service.RecvLine();
		assert(s1 == s2);
		try
		{
			AC::TcpListener Listener(9877);
			AC::TcpClient Service;
			{
				AC::TcpClient Client(AC::InetAddress(_T("localhost"), 9877));
				Service = Listener.Accept();
			}
			std::string s = Service.RecvLine();
		}
		catch(std::exception&)
		{
			assert(GetLastError() == 0); 
		}

		CloseableServer* cs = new CloseableServer();
		AC::Thread Thread(cs);
		Thread.Start();
		Thread.Wait(200);
		cs->CloseServer();
		Thread.Wait();

		AC::Thread ThreadServer(new StressServer());
		ThreadServer.Start();

		int Fails = 0;
		for (int i = 0; i < NUM_CON_STRESS; i++)
		{
			try
			{
				AC::TcpClient Client(AC::InetAddress(_T("localhost"), 3456));

				DisableTimeWait(Client);
				DisableNagle(Client);
				ReuseAddr(Client);
			}
			catch(std::exception&)
			{
				Fails++;
			}
		}
		assert(Fails == 0);
		ThreadServer.Wait();
	}
	catch(std::exception& ex)
	{
		fprintf(stderr, "Error: %s\n", ex.what());
	}
	return 0;
}

