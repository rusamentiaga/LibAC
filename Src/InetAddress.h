#pragma once

#include <winsock2.h>
#include <windows.h>
#include <Ws2tcpip.h>

#include "Exception.h"
#include "tstring.h"

namespace AC
{
	class InetAddress
	{
	protected:
		SOCKADDR_IN m_Addr;

	public:
		InetAddress()
		{
			memset(&m_Addr, 0, sizeof(m_Addr));
		}

		InetAddress(tstring Address, unsigned short Port)
		{
			ADDRINFOT* AddrInfoResult = NULL;

			ADDRINFOT Hints;
			memset(&Hints, 0, sizeof(Hints));
			Hints.ai_family = AF_INET;

			if (GetAddrInfo(Address.c_str(), NULL, &Hints, &AddrInfoResult) != 0)
				throw Exception() << StrError(GetLastError());

			for (ADDRINFOT* ptr = AddrInfoResult; ptr != NULL; ptr = ptr->ai_next)
			{
				if (ptr->ai_family == AF_INET)
				{
					m_Addr = *(struct sockaddr_in *) ptr->ai_addr;
					m_Addr.sin_port = htons(Port);
				}
			}
			FreeAddrInfo(AddrInfoResult);
		}

		InetAddress(unsigned short Port)
		{
			m_Addr.sin_family = AF_INET;
			m_Addr.sin_port = htons(Port);
			m_Addr.sin_addr.s_addr = htonl(INADDR_ANY);
		}

		InetAddress(SOCKADDR_IN Address)
		{
			m_Addr = Address;
		}

		tstring Str() const
		{
			TCHAR IpStr[INET_ADDRSTRLEN];

			if (InetNtop(AF_INET, (PVOID)(&m_Addr.sin_addr), IpStr, INET_ADDRSTRLEN) == NULL)
				throw Exception() << StrError(GetLastError());

			tostringstream os;
			os << IpStr << _T(":") << ntohs(m_Addr.sin_port);
			return os.str();
		}

		operator sockaddr*() const { return (sockaddr*)&m_Addr; }

		bool operator==(const InetAddress& sp) 
		{
			return (Str() == sp.Str());
		}

		int Size() { return sizeof(m_Addr); }
	};
}