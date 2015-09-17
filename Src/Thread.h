#pragma once
#include <windows.h>
#include "Runnable.h"
#include "SynchronizationObject.h"
#include "SharedPtr.h"

namespace AC 
{
	class Thread: public SynchronizationObject
	{
	private:
		static DWORD WINAPI ThreadCallback(LPVOID lpParameter)
		{
			SharedPtr<Runnable> m_Runnable = static_cast<Runnable*>(lpParameter);
			m_Runnable->Run();

			return 0;
		}

	public:
		Thread(Runnable* pRunnable)
		{
			m_hObject = CreateThread(NULL, 0, ThreadCallback, pRunnable, 
				CREATE_SUSPENDED, NULL);
			if (m_hObject == NULL)
			{
				delete pRunnable;
				throw Exception() << StrError(GetLastError());
			}
		}

		void Start()
		{
			if (ResumeThread(m_hObject) == -1)
				throw Exception() << StrError(GetLastError());
		}
	};
}