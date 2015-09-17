#include "stdafx.h"
#include "../../Src/Exception.h"
#include <stdio.h>

const int NUM = 42;

class Number {
protected:
	int m_number;
	
public:
	Number(int N) 
		: m_number(N) {}
		
	void Test() 
	{
		printf("Mi número es %d\n", m_number);
		throw AC::Exception() << "Hubo un error en la clase Number con número " << m_number;
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	Number n(NUM);
	try 
	{
		n.Test();
	}
	catch (std::exception& e)
	{
		fprintf(stderr, "Error en la llamada a n.Test: %s\n", e.what());
	}
	return 0;
}
