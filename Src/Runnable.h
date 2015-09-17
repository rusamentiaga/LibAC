#pragma once

namespace AC
{
	class Runnable
	{
	public:
		Runnable() {};
		virtual ~Runnable() {};

		virtual void Run() = 0;
	};
}