#pragma once
#include "CriticalSection.h"

namespace AC
{
	class ScopedCriticalSection
	{
	private:
		ScopedCriticalSection(const ScopedCriticalSection& objectSrc);	// no implementation
		void operator=(const ScopedCriticalSection& objectSrc);					// no implementation

	protected:
		CriticalSection& m_CriticalSection;

	public:
		ScopedCriticalSection(CriticalSection& cs)
			:m_CriticalSection(cs)
		{
			m_CriticalSection.Enter();
		}
		
		virtual ~ScopedCriticalSection(void)
		{
			m_CriticalSection.Leave();
		}
	};
}
