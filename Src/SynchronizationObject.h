#pragma once
#include <windows.h>
#include "Exception.h"
#include "SharedResource.h"

namespace AC
{
	class SynchronizationObject: public SharedResource<SynchronizationObject>
	{
	protected:
		HANDLE m_hObject;

		virtual void Dispose()
		{
			if (m_hObject != NULL)
			{
				::CloseHandle(m_hObject);
				m_hObject = NULL;
			}
		}

	public:
		SynchronizationObject(void)
		{
			m_hObject = NULL;
		}

		DWORD Wait(DWORD Timeout = INFINITE)
		{
			DWORD dwRet = WaitForSingleObject(m_hObject, Timeout);

			if (dwRet == WAIT_FAILED)
				throw Exception() << StrError(GetLastError());

			return dwRet;
		}

		operator HANDLE() const { return m_hObject; }
	};
}