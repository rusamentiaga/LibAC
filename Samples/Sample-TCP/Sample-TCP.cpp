#include "stdafx.h"

#include "../../Src/ScopedWinSock.h"
#include "../../Src/InetAddress.h"
#include "../../Src/TcpClient.h"
#include "../../Src/TcpListener.h"
#include "../../Src/Thread.h"

#pragma comment(lib, "WS2_32.LIB")

const unsigned short SERVER_PORT = 9876;
const TCHAR* SERVER_NAME = _T("localhost");
const char* MESSAGE = "Hola\n";

class Client: public AC::Runnable
{
protected:
	AC::tstring m_Address;
	unsigned short m_Port;

public:
	Client(AC::tstring Address, unsigned short Port)
		:m_Address(Address), m_Port(Port) {}

	void Run()
	{
		try
		{
			const std::string Msg = MESSAGE;

			AC::TcpClient Client(AC::InetAddress(m_Address, m_Port));
			Client.SendLine(Msg);

			printf("[Client] Sent: %s", Msg.c_str());
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
	unsigned short m_Port;

public:
	Server(unsigned short Port)
		:m_Port(Port) {}

	void Run()
	{
		try
		{
			AC::TcpListener Listener(m_Port);
			AC::TcpClient Client = Listener.Accept();
			std::string Line = Client.RecvLine();

			printf("[Server] Received: %s", Line.c_str());
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

		AC::Thread ThreadServer(new Server(SERVER_PORT));
		AC::Thread ThreadClient(new Client(SERVER_NAME, SERVER_PORT));
	
		ThreadServer.Start();
		ThreadClient.Start();

		ThreadClient.Wait();
		ThreadServer.Wait();
	}
	catch(std::exception& ex)
	{
		fprintf(stderr, "Error: %s\n", ex.what());
	}
	return 0;
}

