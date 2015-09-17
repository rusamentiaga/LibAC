// Test_RefCounter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <vector>
#include <assert.h>

#include "../../Src/SharedResource.h"
#include "../../Src/CriticalSection.h"

int g_AliveResourceTest = 0;

class ResourceTest: public AC::SharedResource<ResourceTest>
{
protected:
	virtual void Dispose()
	{
		g_AliveResourceTest--;
		printf("Deleting ResourceTest %d\n", m_i);
		m_i = -1;
	}

public:
	int m_i;

	ResourceTest(const ResourceTest& rt) 
		:SharedResource<ResourceTest>(rt), m_i(rt.m_i)
	{
	}
	
	ResourceTest& operator=(const ResourceTest& rt) 
	{
		if (this != &rt) 
		{
			SharedResource<ResourceTest>::operator =(rt);
			m_i = rt.m_i;
		}
		return *this;
	}

	ResourceTest(int i) 
		: m_i(i)
    {
		g_AliveResourceTest++;
		printf("Creating ResourceTest %d\n", m_i);
	}

	void Print()
	{
		printf("Printing resource %d\n", m_i);
	}

	int GetRefCount() { return *m_pCount; }
};

int g_AliveCopiableCriticalSection = 0;
class CopiableCriticalSection: public AC::SharedResource<CopiableCriticalSection>
{
protected:
	CRITICAL_SECTION m_CriticalSection;

	virtual void Dispose()
	{
		g_AliveCopiableCriticalSection--;
		printf("Destroying CopiableCriticalSection\n");
		DeleteCriticalSection(&m_CriticalSection);
	}

public:
	CopiableCriticalSection() 
    {
		g_AliveCopiableCriticalSection++;
		printf("Creating CopiableCriticalSection\n");
		InitializeCriticalSection(&m_CriticalSection);
	}
};

int g_AliveDerivedCopiableCriticalSection = 0;

class DerivedCopiableCriticalSection: public CopiableCriticalSection
{
protected:
	void Dispose()
	{
		g_AliveDerivedCopiableCriticalSection--;
		printf("Destroying DerivedCopiableCriticalSection\n");

		CopiableCriticalSection::Dispose();
	}

public:
	DerivedCopiableCriticalSection()
	{
		g_AliveDerivedCopiableCriticalSection++;
		printf("Creating DerivedCopiableCriticalSection\n");
	}
};

void TestResourceTest()
{
	{
		{
			std::vector<ResourceTest> v;
			for (int i = 0; i < 10; i++)
			{
				ResourceTest r(i);
				v.push_back(r);
			}
			for (int i = 0; i < 10; i++)
				v[i].Print();

			for (int i = 0; i < 10; i++)
				assert(v[i].GetRefCount() == 1);
			assert(g_AliveResourceTest == 10);
		}
		assert(g_AliveResourceTest == 0);

		ResourceTest r1(100);
		assert((r1.GetRefCount() == 1) && (r1.m_i == 100));
		assert(g_AliveResourceTest == 1);

		ResourceTest r2(200);
		assert((r1.GetRefCount() == 1) && (r1.m_i == 100));
		assert((r2.GetRefCount() == 1) && (r2.m_i == 200));
		assert(g_AliveResourceTest == 2);

		ResourceTest r3(r1);
		assert((r1.GetRefCount() == 2) && (r1.m_i == 100));
		assert((r2.GetRefCount() == 1) && (r2.m_i == 200));
		assert((r3.GetRefCount() == 2) && (r3.m_i == 100));
		assert(g_AliveResourceTest == 2);

		ResourceTest r4 = r3;
		assert((r1.GetRefCount() == 3) && (r1.m_i == 100));
		assert((r2.GetRefCount() == 1) && (r2.m_i == 200));
		assert((r3.GetRefCount() == 3) && (r3.m_i == 100));
		assert((r4.GetRefCount() == 3) && (r4.m_i == 100));
		assert(g_AliveResourceTest == 2);

		r3 = r2;
		assert((r1.GetRefCount() == 2) && (r1.m_i == 100));
		assert((r2.GetRefCount() == 2) && (r2.m_i == 200));
		assert((r3.GetRefCount() == 2) && (r3.m_i == 200));
		assert((r4.GetRefCount() == 2) && (r4.m_i == 100));
		assert(g_AliveResourceTest == 2);

		r1 = r2;
		assert((r1.GetRefCount() == 3) && (r1.m_i == 200));
		assert((r2.GetRefCount() == 3) && (r2.m_i == 200));
		assert((r3.GetRefCount() == 3) && (r3.m_i == 200));
		assert((r4.GetRefCount() == 1) && (r4.m_i == 100));
		assert(g_AliveResourceTest == 2);

		r4 = r1;
		assert((r1.GetRefCount() == 4) && (r1.m_i == 200));
		assert((r2.GetRefCount() == 4) && (r2.m_i == 200));
		assert((r3.GetRefCount() == 4) && (r3.m_i == 200));
		assert((r4.GetRefCount() == 4) && (r4.m_i == 200));
		assert(g_AliveResourceTest == 1);
	}
	assert(g_AliveResourceTest == 0);
}

void TestCopiableCriticalSection()
{
	{
		CopiableCriticalSection c1;
		assert(g_AliveCopiableCriticalSection == 1);

		CopiableCriticalSection c2;
		assert(g_AliveCopiableCriticalSection == 2);

		CopiableCriticalSection c3 = c2;
		assert(g_AliveCopiableCriticalSection == 2);

		c1 = c2;
		assert(g_AliveCopiableCriticalSection == 1);
	}
	assert(g_AliveCopiableCriticalSection == 0);
}

void TestDerivedCopiableCriticalSection()
{
	{
		DerivedCopiableCriticalSection c1;
		assert(g_AliveCopiableCriticalSection == 1);
		assert(g_AliveDerivedCopiableCriticalSection == 1);

		DerivedCopiableCriticalSection c2;
		assert(g_AliveCopiableCriticalSection == 2);
		assert(g_AliveDerivedCopiableCriticalSection == 2);

		DerivedCopiableCriticalSection c3 = c2;
		assert(g_AliveCopiableCriticalSection == 2);
		assert(g_AliveDerivedCopiableCriticalSection == 2);

		c1 = c2;
		assert(g_AliveCopiableCriticalSection == 1);
		assert(g_AliveDerivedCopiableCriticalSection == 1);
	}
	assert(g_AliveCopiableCriticalSection == 0);
	assert(g_AliveDerivedCopiableCriticalSection == 0); 
}

int _tmain(int /* argc */, _TCHAR* /* argv[]*/)
{
	TestResourceTest();
	TestCopiableCriticalSection();
	TestDerivedCopiableCriticalSection();

	return 0;
}

