#include "stdafx.h"

#include "../../Src/ScopedWinSock.h"
#include "../../Src/InetAddress.h"

#pragma comment(lib, "WS2_32.LIB")

const TCHAR* URL = _T("www.uniovi.es");
const unsigned short PORT = 80;

int _tmain(int argc, _TCHAR* argv[])
{
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);

	// Para que salgan bien los caracteres acentuados
	std::locale::global(std::locale(""));
	try
	{
		AC::ScopedWinSock UsingWinSock;
		AC::InetAddress Address(URL, PORT);

		_tprintf(_T("Dirección: %s\n"), Address.Str().c_str());
	}
	catch(std::exception& ex)
	{
		fprintf(stderr, "Error: %s\n", ex.what());
	}
	return 0;
}

