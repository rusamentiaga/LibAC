#pragma once

#include "Socket.h"
#include "TcpClient.h"

namespace AC
{
	class TcpListener: public TcpSocket
	{
	public:
		TcpListener() {}

		TcpListener(InetAddress Addr)
		{
			Listen(Addr);
		}

		void Listen(InetAddress Addr)
		{
			if (bind(m_Socket, Addr, Addr.Size()) == SOCKET_ERROR)
				throw Exception() << StrError(GetLastError());

			if (listen(m_Socket, SOMAXCONN) == SOCKET_ERROR)
				throw Exception() << StrError(GetLastError());
		}

		TcpClient Accept()
		{
			SOCKET s = accept(m_Socket, NULL, NULL);
			if (s == INVALID_SOCKET)
				throw Exception() << StrError(GetLastError());

			return TcpClient(s);
		}
	};
}
