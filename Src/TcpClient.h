#pragma once

#include "TcpSocket.h"
#include "InetAddress.h"
#include "CriticalSection.h"
#include "ScopedCriticalSection.h"

namespace AC
{
	class TcpClient: public TcpSocket
	{
	protected:
		tstring GetName(int (WSAAPI *sockfunc)(SOCKET, struct sockaddr*, int*))
		{
			InetAddress Addr;
			int AddrSize = Addr.Size();

			if (sockfunc(m_Socket, Addr, &AddrSize) == SOCKET_ERROR)
				throw Exception() << StrError(GetLastError());
			return Addr.Str();
		}

	public:
		TcpClient() {}

		TcpClient(InetAddress Addr)
		{
			Connect(Addr);
		}
		
		TcpClient(SOCKET s)
			:TcpSocket(s)	{}

		void Connect(InetAddress Addr)
		{
			if (connect(m_Socket, Addr, Addr.Size()) == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSAEWOULDBLOCK)
					throw Exception() << StrError(GetLastError());
			}
		}

		int Recv(char *buf, int len, int flags = 0)
		{
			int ret = recv(m_Socket, buf, len, flags);
			if ((ret == SOCKET_ERROR) || (ret == 0))
				throw Exception() << StrError(GetLastError());
			return ret;
		}

		int Send(const char *buf, int len, int flags = 0)
		{
			int ret = send(m_Socket, buf, len, flags);
			if (ret == SOCKET_ERROR)
				throw Exception() << StrError(GetLastError());
			return ret;
		}

		std::string RecvLine(const char* EndLine = "\n")
		{
			std::string Line;
			char c = 0;

			while (Line.find(EndLine, 0) == std::string::npos)
			{
				Recv(&c, sizeof(char));
				Line += c;
			}
			return Line;
		}

		void SendLine(std::string Line)
		{
			if (Line.empty())
				return;

			char *sendbuf = &Line[0];
			int len = Line.size();

			int nLeft = len;
			while (nLeft > 0)
			{
				int ret = Send(&sendbuf[len - nLeft], nLeft);
				nLeft -= ret;
			}
		}

		tstring GetPeerName() { return GetName(getpeername); }
		tstring GetSockName() { return GetName(getsockname); }
	};
}
