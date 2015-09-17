#pragma once

#include <winsock2.h>
#include <windows.h>

#include "Exception.h"

namespace AC
{
	class ScopedWinSock
	{
	public:
		ScopedWinSock()
		{
		  WSADATA wsaData;
			const WORD WinSockVersion = MAKEWORD(2, 2);

			if (WSAStartup(WinSockVersion, &wsaData) != 0)
				throw Exception() << StrError(GetLastError());
		}
		
		~ScopedWinSock()
		{
			WSACleanup();
		}
	};
}