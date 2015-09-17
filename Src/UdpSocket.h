#pragma once

#include "Socket.h"

namespace AC
{
	class UdpSocket: public Socket
	{
	public:
		UdpSocket()
			:Socket(SOCK_DGRAM, IPPROTO_UDP) {}
	};
}