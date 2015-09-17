#pragma once
#include "SynchronizationObject.h"
#include <assert.h>
#include "Exception.h"

namespace AC
{
	class Semaphore: public SynchronizationObject
	{
	public:
		Semaphore(LONG lInitialCount = 1, LONG lMaxCount = 1,
			LPCTSTR pstrName=NULL, LPSECURITY_ATTRIBUTES lpsaAttributes = NULL)
		{
			assert(lMaxCount > 0);
			assert(lInitialCount <= lMaxCount);

			m_hObject = ::CreateSemaphore(lpsaAttributes, lInitialCount, lMaxCount,	pstrName);
			if (m_hObject == NULL)
				throw Exception() << StrError(GetLastError());
		}

		void Release(LONG lCount, LPLONG lpPrevCount = NULL)
		{
			if (ReleaseSemaphore(m_hObject, lCount, lpPrevCount) == FALSE)
				throw Exception() << StrError(GetLastError());
		}
	};
}
