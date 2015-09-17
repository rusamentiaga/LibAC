#pragma once
#include "SharedQueue.h"
#include "Thread.h"
#include "SharedPtr.h"

namespace AC
{
	class ThreadPool
	{
	protected:
		typedef SharedPtr<Runnable> RunnablePtr;
		SharedQueue<RunnablePtr> m_SharedQueue;
		std::vector<Thread> m_WorkerThreads;

		class RunnableTask: public Runnable
		{
		protected:
			SharedQueue<RunnablePtr>* m_SharedQueue;

		public:
			RunnableTask(SharedQueue<RunnablePtr>* Queue): m_SharedQueue(Queue) {}

			virtual void Run()
			{
				try
				{
					RunnablePtr Task;
					while ((Task = m_SharedQueue->Pop()) != NULL)
						Task->Run();
				}
				catch(...) {}
			}
		};

	public:
		ThreadPool(int NumThreads, int QueueSize)
			: m_SharedQueue(QueueSize)
		{
			for (int i = 0; i < NumThreads; i++)
			{
				Thread WorkerThread(new RunnableTask(&m_SharedQueue));
				WorkerThread.Start();
				m_WorkerThreads.push_back(WorkerThread);
			}
		}

		virtual ~ThreadPool(void)
		{
			for (unsigned int i = 0; i < m_WorkerThreads.size(); i++)
				AddTask(NULL);
			for (unsigned int i = 0; i < m_WorkerThreads.size(); i++)
				m_WorkerThreads[i].Wait();
		}

		void AddTask(RunnablePtr pRunnable)
		{
			m_SharedQueue.Push(pRunnable);
		}
	};
}
