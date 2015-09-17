#pragma once

#include <iostream>
#include <sstream>
#include <memory>
#include <stdexcept>
#include "tstring.h"


namespace AC
{
	class Exception : public std::runtime_error
	{
	private:
		mutable std::auto_ptr<tostringstream> m_ss;
		mutable std::string m_s;

	public:
		Exception()
			: std::runtime_error(""),
			m_ss(std::auto_ptr<tostringstream>(new tostringstream())) {}

		virtual ~Exception() throw() {}

		template <typename T>
		Exception & operator << (const T & t)
		{
			(*m_ss) << t;
			return *this;
		}

		virtual const char * what() const throw()
		{
			try
			{
				tstring s = m_ss->str();
				m_s = tstringTostring(s);
				return m_s.c_str();
			}
			catch (...)
			{
				return "Exception::what() threw an exception";
			}
		}
	};
}
