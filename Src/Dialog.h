#pragma once

#include "Window.h"

namespace AC 
{
	class Dialog: public Window
	{
	protected:
		int m_Template;

		static	LRESULT CALLBACK DialogProc(HWND hWnd, UINT uMsg, 
											WPARAM wParam, LPARAM lParam)
		{
			Dialog *self = reinterpret_cast<Dialog*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

			if (uMsg == WM_INITDIALOG) 
			{
				self = reinterpret_cast<Dialog*>(lParam);
				self->m_hWnd = hWnd;
				SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));
			} 
			if (self) 
				return self->OnMessage(uMsg, wParam, lParam);
			else 
				return 0;
		}

		virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
		{
			MsgHandler pfn = FindMessageHandler(message, wParam, lParam);

			if (pfn)
				(this->*pfn)(wParam, lParam);
			return 0;
		}

	public:
		Dialog(int Template): m_Template(Template) {}

		INT_PTR DoModal(HWND hWndParent, HINSTANCE hInstance)
		{
			return ::DialogBoxParam(hInstance, reinterpret_cast<LPCTSTR>(m_Template), hWndParent, 
				reinterpret_cast<DLGPROC>(DialogProc), reinterpret_cast<LPARAM>(this));
		}

		void EndDialog(int nEndValue)
		{
			::EndDialog(*this, nEndValue);
		}
	};
}
