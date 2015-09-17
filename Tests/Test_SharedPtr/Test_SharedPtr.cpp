// Test_SharedPtr.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <assert.h>
#include <map>

#include "../../Src/SharedPtr.h"

int g_AliveData = 0;

template<typename T>
class SharedPtrTest: public AC::SharedPtr<T>
{
public:
	int GetRefCount() { return *m_pCount; }

	SharedPtrTest(T* pT = NULL)
	{
		m_pT = pT;
	}

	SharedPtrTest(const SharedPtrTest& sp) 
		:SharedPtr(sp)
	{
	}
};


class Data
{
	int _i;
public:
	Data(int i): _i(i)
	{
		g_AliveData++;
	}

	~Data()
	{
		g_AliveData--;
	}

	int GetI() { return _i; };
};

class Map
{
	std::map< int, AC::SharedPtr<Data> > m_Map;
public:
	void Register(int i, Data* d)
	{
		m_Map[i] = d;
	}
};

void TestSharedPtr()
{
	{
		SharedPtrTest<Data> p1 = new Data(10);
		assert(p1.GetRefCount() == 1);

		SharedPtrTest<Data> p2 = p1;
		assert(p1.GetRefCount() == 2);

		SharedPtrTest<Data> p3;
		assert(g_AliveData == 1);

		p3 = p1;
		assert(g_AliveData == 1);
		assert(p1->GetI() == p3->GetI());

		*p2 = 25;
		assert(p1->GetI() == 25);
		assert(p1->GetI() == p3->GetI());
		assert(p3->GetI() == p2->GetI());

		p2 = new Data(26);
		assert(g_AliveData == 2);
		assert(p1->GetI() == p3->GetI());
		assert(p1->GetI() == 25);
		assert(p2->GetI() == 26);
	}
	assert(g_AliveData == 0);

	{
		std::map<int, AC::SharedPtr<Data> > m;

		m[0] = new Data(99);
		assert(g_AliveData == 1);

		m[2] =  new Data(999);
		assert(g_AliveData == 2);

		m[0] = m[2];
		assert(g_AliveData == 1);
	}
	assert(g_AliveData == 0);

	{
		Map m;

		m.Register(0, new Data(0));
		assert(g_AliveData == 1);

		m.Register(1, new Data(1));
		assert(g_AliveData == 2);
	}
	assert(g_AliveData == 0);
}

int _tmain(int argc, _TCHAR* argv[])
{
	TestSharedPtr();

	return 0;
}

