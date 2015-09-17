#include "stdafx.h"

#include "../../Src/ScopedWinSock.h"
#include "../../Src/UdpClient.h"
#include "../../Src/UdpServer.h"
#include "../../Src/Thread.h"

#pragma comment(lib, "WS2_32.LIB")

const unsigned short SERVER_PORT = 9876;
const TCHAR* SERVER_NAME = _T("localhost");

const int MESSAGE_DATA = 5;

class Client: public AC::Runnable
{
protected:
	AC::InetAddress m_ServerAddr;

public:
	Client(AC::InetAddress ServerAddr)
		:m_ServerAddr(ServerAddr) {}

	void Run()
	{
		try
		{
			AC::UdpClient Client;

			const int Data = MESSAGE_DATA;
			Client.SendTo(Data, m_ServerAddr);

			_tprintf(_T("[Client] Sent: %d to %s\n"), Data, m_ServerAddr.Str().c_str());
		}
		catch(std::exception& ex)
		{
			fprintf(stderr, "[Client] Error: %s\n", ex.what());
		}
	}
};

class Server: public AC::Runnable
{
protected:
	AC::InetAddress m_ServerAddr;

public:
	Server(AC::InetAddress ServerAddr)
		:m_ServerAddr(ServerAddr) {}

public:
	Server() {}

	void Run()
	{
		try
		{
			AC::UdpServer Server(m_ServerAddr);

			AC::InetAddress From;
			int Data;
			Server.RecvFrom(Data, From);

			_tprintf(_T("[Server] Received: %d from %s\n"), Data, From.Str().c_str());
		}
		catch(std::exception& ex)
		{
			fprintf(stderr, "[Server] Error: %s\n", ex.what());
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

		AC::InetAddress m_ServerAddr(SERVER_NAME, SERVER_PORT);
		AC::Thread ThreadServer(new Server(m_ServerAddr));
		AC::Thread ThreadClient(new Client(m_ServerAddr));
	
		ThreadServer.Start();
		ThreadClient.Start();

		ThreadClient.Wait();
		ThreadServer.Wait();
	}
	catch(std::exception& ex)
	{
		fprintf(stderr, "Error: %s\n", ex.what());
	}
}

