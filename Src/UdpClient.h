#pragma once

#include <winsock2.h>
#include <windows.h>

#include "UdpSocket.h"
#include "InetAddress.h"


namespace AC
{
	class UdpClient: public UdpSocket
	{
	public:
		template <typename T>
		int SendTo(const T& t, InetAddress Addr)
		{
			char* sendbuff = (char*)&t;
			int nLeft = sizeof(t);

			int ret = sendto(m_Socket, sendbuff, nLeft, 0, Addr, Addr.Size());
			if (ret == SOCKET_ERROR)
				throw Exception() << StrError(GetLastError());
			return ret;
		}

		template <typename T>
		int RecvFrom(T& t, InetAddress& Addr)
		{
			int nLeft = sizeof(t);
			char* recvbuf = (char*)&t;
			SOCKADDR_IN Address;
			int AddressSize = sizeof(Address);

			int ret = recvfrom(m_Socket, recvbuf, nLeft, 0, (SOCKADDR *)&Address, &AddressSize);
			Addr = InetAddress(Address);

			if (ret == SOCKET_ERROR)
				throw Exception() << StrError(GetLastError());

			return ret;
		}
	};
}