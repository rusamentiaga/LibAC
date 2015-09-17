#pragma once
#include <windows.h>

#include "SharedResource.h"

namespace AC
{
	class CriticalSection: public SharedResource<CriticalSection>
	{
	protected:
		CRITICAL_SECTION m_CriticalSection;

		virtual void Dispose(void)
		{
			DeleteCriticalSection(&m_CriticalSection);
		}

	public:
		CriticalSection(void)
		{
			InitializeCriticalSection(&m_CriticalSection);
		}

		void Enter()
		{
			EnterCriticalSection(&m_CriticalSection);
		}

		void Leave()
		{
			LeaveCriticalSection(&m_CriticalSection);
		}
	};
}
