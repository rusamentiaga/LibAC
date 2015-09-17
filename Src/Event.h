#pragma once
#include "SynchronizationObject.h"
#include "Exception.h"

namespace AC
{
	class Event: public SynchronizationObject
	{
	public:
		Event(BOOL bInitiallyOwn = FALSE, BOOL bManualReset = FALSE,
			LPCTSTR lpszNAme = NULL, LPSECURITY_ATTRIBUTES lpsaAttribute = NULL)
		{
			m_hObject = ::CreateEvent(lpsaAttribute, bManualReset, bInitiallyOwn, lpszNAme);
			if (m_hObject == NULL)
				throw Exception() << StrError(GetLastError());
		}

		void SetEvent()		
		{ 
			if(!::SetEvent(m_hObject))
				throw Exception() << StrError(GetLastError());
		}
		void ResetEvent()	
		{ 
			if(!::ResetEvent(m_hObject))
				throw Exception() << StrError(GetLastError()); 
		}
	};
};
