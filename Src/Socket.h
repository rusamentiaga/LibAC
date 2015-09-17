#pragma once

#include <winsock2.h>
#include <windows.h>
#include "Exception.h"
#include "Event.h"
#include "SharedResource.h"
#include "tstring.h"

namespace AC
{
	class Socket: public SharedResource<Socket>
	{
	protected:
		SOCKET m_Socket;

		virtual void Dispose()
		{
			if (m_Socket != INVALID_SOCKET)
			{
				closesocket(m_Socket);
				m_Socket = INVALID_SOCKET;
			}
		}

	public:
		Socket(int Type, int Protocol)
		{
			m_Socket = socket(AF_INET, Type, Protocol);
			if (m_Socket == INVALID_SOCKET)
				throw Exception() << StrError(GetLastError());
		}

		Socket(SOCKET s)
			:m_Socket(s) {}

		Socket(const Socket& rhs) 
			:SharedResource<Socket>(rhs), m_Socket(rhs.m_Socket) {}

		void Close()
		{
			Dispose();
		}

		void EventSelect(const Event& SocketEvent, long lNetworkEvents)
		{
			if (WSAEventSelect(m_Socket, SocketEvent, lNetworkEvents) == SOCKET_ERROR)
				throw Exception() << StrError(GetLastError());
		}

		void AsyncSelect(HWND hWnd, unsigned int wMsg, long lEvent)
		{
			if (WSAAsyncSelect(m_Socket, hWnd, wMsg, lEvent) == SOCKET_ERROR)
				throw Exception() << StrError(GetLastError());
		}

		void SetBlocking(BOOL status)
		{
			u_long dwNonBlocking = !status;
			if (ioctlsocket(m_Socket, FIONBIO, &dwNonBlocking) == SOCKET_ERROR)
				throw Exception() << StrError(GetLastError());
		}

		void EnumNetworkEvents(Event& SocketEvent, LPWSANETWORKEVENTS lpNetworkEvents)
		{
			if (WSAEnumNetworkEvents(m_Socket, SocketEvent, lpNetworkEvents) == SOCKET_ERROR)
				throw Exception() << StrError(GetLastError());
		}
		
		operator SOCKET() const { return m_Socket; }
	};
}
