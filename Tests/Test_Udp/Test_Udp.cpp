// Test_Udp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "../../Src/ScopedWinSock.h"
#include "../../Src/Socket.h"
#include "../../Src/UdpClient.h"
#include "../../Src/UdpServer.h"

#pragma comment(lib, "WS2_32.LIB")

int _tmain(int argc, _TCHAR* argv[])
{
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);

	try
	{
		AC::ScopedWinSock UsingWinSock;
		AC::InetAddress ServerAddr(_T("127.0.0.1"), 9000);
		AC::UdpServer Server(ServerAddr);
		AC::UdpClient Client;
		AC::InetAddress ClientAddressFrom;
		AC::InetAddress ServerAddressFrom;

		int SendData = 25;
		int RecvData;

		Client.SendTo(SendData, ServerAddr);
		Server.RecvFrom(RecvData, ClientAddressFrom);
		_tprintf(_T("Server Data received from %s\n"), 
			ClientAddressFrom.Str().c_str());
		assert(RecvData == SendData);

		Client.SendTo(SendData, ServerAddr);
		Server.RecvFrom(RecvData, ClientAddressFrom);
		_tprintf(_T("Server Data received from %s\n"), 
			ClientAddressFrom.Str().c_str());
		assert(RecvData == SendData);

		Server.SendTo(SendData, ClientAddressFrom);
		Client.RecvFrom(RecvData, ServerAddressFrom);
		_tprintf(_T("Client Data received from %s\n"), 
			ServerAddressFrom.Str().c_str());
		assert(RecvData == SendData);

		Server.SendTo(SendData, ClientAddressFrom);
		Client.RecvFrom(RecvData, ServerAddressFrom);
		_tprintf(_T("Client Data received from %s\n"), 
			ServerAddressFrom.Str().c_str());
		assert(RecvData == SendData);

		AC::InetAddress NewClientAddressFrom;
		AC::UdpClient NewClient;

		NewClient.SendTo(SendData, ServerAddr);
		Server.RecvFrom(RecvData, NewClientAddressFrom);
		_tprintf(_T("Server Data received from %s\n"), 
			NewClientAddressFrom.Str().c_str());
		assert(RecvData == SendData);

		Client.Close();
		try
		{
			for (;;)
			{
				Server.SendTo(SendData, ClientAddressFrom);
				Server.RecvFrom(RecvData, ClientAddressFrom);
			}
		}
		catch(...)
		{
			_tprintf(_T("Client connection closed %s\n"), 
				ClientAddressFrom.Str().c_str());
		}

		/*
		Client.SetBlocking(FALSE);
		struct Package{ char Data[1024]; } KB ={0};
		for (int i = 0; i < 1024*1024; i++)
		{
			if (i%1024 == 0)
				printf("Sent %d MB\n", i/1024);
			Client.SendTo(KB, ServerAddr);
		}
		*/
	}
	catch(std::exception& ex)
	{
		fprintf(stderr, "Exception: %s\n", ex.what());
	}

	return 0;
}

