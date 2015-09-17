#pragma once
#include <windows.h>
#include "CriticalSection.h"
#include "ScopedCriticalSection.h"
#include "Semaphore.h"
#include <vector>

namespace AC
{
	template <class T>
	class SharedQueue
	{
	protected:
		std::vector<T> m_QueueData;

		int m_PopIndex;
		int m_PushIndex;

		int m_Size;

		CriticalSection m_csPopIndex;
		CriticalSection m_csPushIndex;

		Semaphore m_FreeSlots;
		Semaphore m_AvailableItems;

	public:
		SharedQueue(int Size = 1)
			:m_QueueData(Size), m_FreeSlots(Size, Size), m_AvailableItems(0, Size),
			m_Size(Size), m_PopIndex(0), m_PushIndex(0) {}

		DWORD Push(const T& Item, DWORD Timeout = INFINITE)
		{
			DWORD dwRet = m_FreeSlots.Wait(Timeout);
			if (dwRet != WAIT_TIMEOUT)
			{
				{
					ScopedCriticalSection Lock(m_csPushIndex);
					m_QueueData[m_PushIndex] = Item;
					m_PushIndex = (m_PushIndex + 1) % m_Size;
				}
				m_AvailableItems.Release(1);
			}
			return dwRet;
		}

		T Pop()
		{
			m_AvailableItems.Wait(INFINITE);
			T item;
			{
				ScopedCriticalSection Lock(m_csPopIndex);
				item = m_QueueData[m_PopIndex];
				m_PopIndex = (m_PopIndex + 1) % m_Size;
			}
			m_FreeSlots.Release(1);

			return item;
		}
	};
}
