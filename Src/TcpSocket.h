#pragma once

#include "Socket.h"

namespace AC
{
	class TcpSocket: public Socket
	{
	public:
		TcpSocket()
			:Socket(SOCK_STREAM, IPPROTO_TCP) {}
		
		TcpSocket(SOCKET s)
			:Socket(s) {}
	};
}
