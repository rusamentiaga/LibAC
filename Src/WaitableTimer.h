#pragma once

#include <windows.h>

#include "SynchronizationObject.h"
#include "Exception.h"

namespace AC
{
	class WaitableTimer: public SynchronizationObject
	{
	public:
		WaitableTimer()
		{
			m_hObject = CreateWaitableTimer(NULL, FALSE, NULL);
			if (m_hObject == NULL)
				throw Exception() << StrError(GetLastError());
		}

		void StartMilliseconds(int Period)
		{
			const int ConversionFactor = 10000;
			LARGE_INTEGER FirstTimerPeriod;

			FirstTimerPeriod.QuadPart = -(LONGLONG)Period*ConversionFactor;
			if(SetWaitableTimer(m_hObject, &FirstTimerPeriod, Period, 
				NULL, NULL, 0) == 0)
			{
				throw Exception() << StrError(GetLastError());
			}
		}

		void Stop()
		{
			if (CancelWaitableTimer(m_hObject) == 0)
				throw Exception() << StrError(GetLastError());
		}
	};
}