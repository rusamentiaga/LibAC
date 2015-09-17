#pragma once
#include <Windows.h>

namespace AC
{
	template <class T>
	class SharedResource
	{
	protected:
		// DerivedResource sirve para llamar a la funci�n Dispose de la clase 
		// que herede de SharedResource. Se utiliza porque no se puede llamar 
		// a funciones virtuales desde el destructor de una clase
		struct DerivedResource: public T
		{
			DerivedResource() {}
			static void DoDispose(T& t)
			{
				void (T::*fn)() = &DerivedResource::Dispose;
				(t.*fn)();
			}
		};

		// Contador de referencias
		// Se realiza un acceso sincronizado desde varios hilos
		LONG* m_pCount;

		// Decrementa el n�mero de referencias
		// Si se llega a cero se llama a la funci�n Dispose que libera
		// el recurso compartido
		void Release()
		{
			if (!InterlockedDecrement(m_pCount))
			{
				delete m_pCount;
				DerivedResource::DoDispose(static_cast<T&>(*this));
			}
		}

		// Incrementa el n�mero de referencias
		void Acquire(const SharedResource& rhs)
		{
			m_pCount = rhs.m_pCount;
			InterlockedIncrement(m_pCount);
		}

	public:
		SharedResource() : m_pCount(new LONG(1)) {}

		SharedResource(const SharedResource<T>& rhs) 
		{
			Acquire(rhs);
		}

		SharedResource& operator=(const SharedResource<T>& rhs)
		{
			if (this != &rhs) 
			{
				Release();
				Acquire(rhs);
			}
			return *this;
		}

		~SharedResource()
		{
			Release();
		}
	};
}