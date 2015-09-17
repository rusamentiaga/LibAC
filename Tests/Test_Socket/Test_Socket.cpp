// Test_Socket.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "../../Src/Socket.h"
#include "../../Src/InetAddress.h"
#include "../../Src/ScopedWinSock.h"
#include "../../Src/TcpClient.h"
#include "../../Src/TcpListener.h"
#include "../../Src/Thread.h"
#include "../../Src/StopWatch.h"

#pragma comment(lib, "WS2_32.LIB")

const int NUM_CONNECTIONS = 10000;

class Worker: public AC::Runnable
{
protected:
	AC::TcpClient m_Client;
	int m_NumConnection;

public:
	Worker(AC::TcpClient Client, int NumConnection)
		:m_Client(Client), m_NumConnection(NumConnection)
	{
	}

	void Run()
	{
		try
		{
			int Data;
			m_Client.Recv((char*)&Data, sizeof(Data));
			assert(Data == m_NumConnection);

			Data += Data;
			m_Client.Send((char*)&Data, sizeof(Data));
		}
		catch(std::exception& ex)
		{
			fprintf(stderr, "[Worker thread] Error: %s\n", ex.what());
		}
	}
};


class Server: public AC::Runnable
{
protected:
	unsigned short m_Port;

public:
	Server(unsigned short Port)
		:m_Port(Port)
	{
	}

	void Run()
	{
		try
		{
			AC::TcpListener Listener(m_Port);

			for (int i = 0; i  < NUM_CONNECTIONS; i++)
			{
				AC::TcpClient Client = Listener.Accept();

				AC::Thread WorkerThread(new Worker(Client, i));
				WorkerThread.Start();
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
	AC::tstring m_Address;
	unsigned short m_Port;

public:
	Client(AC::tstring Address, unsigned short Port)
		:m_Address(Address), m_Port(Port)
	{
	}

public:
	void Run()
	{
		try
		{
			for (int i = 0; i  < NUM_CONNECTIONS; i++)
			{
				AC::TcpClient Client(AC::InetAddress(m_Address, m_Port));
				Client.Send((char*)&i, sizeof(i));

				int Data;
				Client.Recv((char*)&Data, sizeof(Data));
				assert(Data == i*2);
			}
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

		AC::Thread ThreadServer(new Server(9876));
		AC::Thread ThreadClient(new Client(_T("localHost"), 9876));
	
		ThreadServer.Start();

		Sleep(100);

		AC::StopWatch StopWatch;

		ThreadClient.Start();

		ThreadClient.Wait();
		Sleep(2000);
		ThreadServer.Wait();
		printf("Test took: %I64d milliseconds\n", StopWatch.ElapsedMilliseconds());
	}
	catch(std::exception& ex)
	{
		fprintf(stderr, "Error: %s\n", ex.what());
	}

	return 0;
}

