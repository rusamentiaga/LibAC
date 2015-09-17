#pragma once

#include <Windows.h>
#include "Exception.h"

namespace AC
{
	class StopWatch
	{
	protected:
		LARGE_INTEGER m_liPerfFreq; 
		LARGE_INTEGER m_liPerfStart;

		__int64 ElapsedTime(int UnitsFactor) const
		{
			LARGE_INTEGER liPerfNow;
			QueryPerformanceCounter(&liPerfNow);
			return(((liPerfNow.QuadPart - m_liPerfStart.QuadPart) * UnitsFactor)
				/ m_liPerfFreq.QuadPart);
		}

	public:
		StopWatch() 
		{ 
			if (QueryPerformanceFrequency(&m_liPerfFreq) == 0)
				throw Exception() << StrError(GetLastError());
			Start(); 
		}

		void Start() 
		{ 
			QueryPerformanceCounter(&m_liPerfStart); 
		}

		__int64 ElapsedMilliseconds() const 
		{
			const int FactorToMillisecondss = 1000;
			return ElapsedTime(FactorToMillisecondss);
		}

		__int64 ElapsedMicroseconds() const 
		{
			const int FactorToMicroseconds = 1000000;
			return ElapsedTime(FactorToMicroseconds);
		}
	};
}