#pragma once
#include "SharedResource.h"

namespace AC
{
	template<typename T>
	class SharedPtr: public SharedResource< SharedPtr<T> >
	{
	protected:
		T* m_pT;

		virtual void Dispose()
		{
			delete m_pT;
			m_pT = NULL;
		}

	public:
		SharedPtr(T* pT = NULL)
		  : m_pT(pT) {}

		SharedPtr(const SharedPtr& sp) 
			: SharedResource<SharedPtr>(sp), m_pT(sp.m_pT) {}

		bool operator==(const SharedPtr& sp) 
		{
			return m_pT == sp.m_pT;
		}

		bool operator!=(const SharedPtr& sp) 
		{
			return m_pT != sp.m_pT;
		}

		SharedPtr& operator=(const SharedPtr& sp) 
		{
			if (this != &sp) 
			{
				SharedResource<SharedPtr>::operator =(sp);
				m_pT = sp.m_pT;
			}
			return *this;
		}
		T* operator->() const { return m_pT; }
		T& operator*() const { return *m_pT; }
	};
}