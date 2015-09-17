#pragma once

#include <winsock2.h>
#include <windows.h>

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
			std::string AddressAnsi = tstringTostring(Address);

			m_Addr.sin_family = AF_INET;
			m_Addr.sin_port = htons(Port);
			m_Addr.sin_addr.s_addr = inet_addr(AddressAnsi.c_str());

			if (m_Addr.sin_addr.s_addr == INADDR_NONE)
			{
				struct hostent *hp;
				hp = (struct hostent *)gethostbyname(AddressAnsi.c_str());
				if (hp == NULL)
					throw Exception() << StrError(GetLastError());
				else 
					memcpy((void*)&m_Addr.sin_addr,(void *)hp->h_addr_list[0], 
						sizeof(m_Addr.sin_addr));
			}
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
			tostringstream os;
			os << inet_ntoa(m_Addr.sin_addr) << _T(":") << ntohs(m_Addr.sin_port);
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