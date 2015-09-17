#pragma once

#include <winsock2.h>
#include <windows.h>

#include "../../Src/Socket.h"

namespace AC
{
	class UdpServer: public UdpClient
	{
	public:
		UdpServer(InetAddress Addr)
		{
			if (bind(m_Socket, Addr, Addr.Size()) == SOCKET_ERROR)
				throw Exception() << StrError(GetLastError());
		}
	};
}