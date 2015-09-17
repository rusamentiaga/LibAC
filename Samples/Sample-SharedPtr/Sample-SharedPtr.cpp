#include "stdafx.h"

#include "../../Src/SharedPtr.h"

class Data
{
public:
	Data()
	{
		printf("Me creo\n");
	}

	~Data()
	{
		printf("Me destruyo\n");
	}
};

AC::SharedPtr<Data> SharedPtrSample() 
{
	AC::SharedPtr<Data> p1 = new Data;
	AC::SharedPtr<Data> p2 = p1;

	printf ("Salgo de SharedPtrSample\n");
	return p2;
}

int _tmain(int argc, _TCHAR* argv[])
{
	AC::SharedPtr<Data> p3 = SharedPtrSample();

	// p3 es v�lido y no hace falta hacer el delete porque se destruir� 
	// autom�ticamente al salir de _tmain

	return 0;
}