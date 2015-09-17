#pragma once
#include <windows.h>
#include <iostream>
#include <vector>
#include "SharedResource.h"

#include "tstring.h"
#include "Exception.h"

namespace AC 
{
	class Window: public SharedResource<Window>
	{
	protected:
		HWND m_hWnd;
		WNDPROC m_pPrevWndProc;

		virtual void Dispose()
		{
			if (m_hWnd != NULL)
				DestroyWindow();
		}

		typedef LRESULT (Window::*MsgHandler)(WPARAM wParam, LPARAM lParam);
		struct MsgHandlerEntry
		{
			UINT nMessage;	// windows message
			MsgHandler pfn;	// routine to call 
			int nLoWParam; int nHiWParam; 
			int nLoLParam; int nHiLParam; 
		};
		std::vector<MsgHandlerEntry> m_MsgHandlers;

		static const int AnyValue = -1;
		bool MatchMessageParam(int StoredParam, int ReceivedParam)
		{
			if ((StoredParam == AnyValue) || (StoredParam == ReceivedParam))
				return true;
			return false;
		}

		static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, 
										WPARAM wParam, LPARAM lParam)
		{
			Window *self = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

			if (uMsg == WM_NCCREATE) 
			{
				LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
				self = reinterpret_cast<Window*>(lpcs->lpCreateParams);
				self->m_hWnd = hWnd;
				SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));
			} 

			if (self) 
			{
				try
				{
					return self->OnMessage(uMsg, wParam, lParam);
				}
				catch(std::exception& ex)
				{
					tstring Reason = AnsiTotstring(ex.what());
					MessageBox(*self, Reason.c_str(), _T("OnMessage failure"), MB_OK|MB_ICONERROR);
					return EXIT_FAILURE;
				}
			}
			else 
				return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}

		virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
		{
			MsgHandler pfn = FindMessageHandler(message, wParam, lParam);

			if (m_pPrevWndProc) // Subclassing ?
			{
				if (pfn)
					(this->*pfn)(wParam, lParam);
				return CallWindowProc(m_pPrevWndProc, m_hWnd, message, wParam, lParam);
			}
			if (pfn)
				return (this->*pfn)(wParam, lParam);
			else
				return DefWindowProc(m_hWnd, message, wParam, lParam);
		}

		MsgHandler FindMessageHandler(UINT message, WPARAM wParam, LPARAM lParam)
		{
			int nLoWParam = LOWORD(wParam);
			int nHiWParam = HIWORD(wParam);
			int nLoLParam = LOWORD(lParam);
			int nHiLParam = HIWORD(lParam);

			for (unsigned int i = 0; i < m_MsgHandlers.size(); i++)
			{
				if ((m_MsgHandlers[i].nMessage == message) &&
					MatchMessageParam(m_MsgHandlers[i].nLoWParam, nLoWParam) &&
					MatchMessageParam(m_MsgHandlers[i].nHiWParam, nHiWParam) &&
					MatchMessageParam(m_MsgHandlers[i].nLoLParam, nLoLParam) &&
					MatchMessageParam(m_MsgHandlers[i].nHiLParam, nHiLParam))
						return m_MsgHandlers[i].pfn;
			}
			return NULL;
		}

		void AddMessageHandler(UINT nMessage, MsgHandler pfn, 
						int nLoWParam = AnyValue, int nHiWParam = AnyValue, 
						int nLoLParam = AnyValue, int nHiLParam = AnyValue)
		{
			Window::MsgHandlerEntry Entry = {
				nMessage, pfn, nLoWParam, nHiWParam, nLoLParam, nHiLParam };
			m_MsgHandlers.push_back(Entry);
		}

	public:
		Window(): m_hWnd(NULL), m_pPrevWndProc(NULL) {}
		virtual ~Window() {}

		BOOL DestroyWindow()
		{
			BOOL bResult = TRUE;

			if (m_hWnd != NULL)
				bResult = ::DestroyWindow(m_hWnd);

			m_hWnd = NULL;
			m_pPrevWndProc = NULL;

			return bResult;
		}

		void RegisterClass(HINSTANCE hInst, LPCTSTR lpszClassName, UINT nIDMenu = NULL,
							DWORD dwStyle = CS_HREDRAW | CS_VREDRAW, 
							HCURSOR hCursor = LoadCursor(NULL, IDC_ARROW), 
							HBRUSH hbrBg = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1),
							HICON hIcon = LoadIcon(NULL, IDI_APPLICATION))
		{
			WNDCLASSEX wcx = {0};

			wcx.cbSize = sizeof(wcx);  
			wcx.hInstance = hInst;  
			wcx.lpszClassName = lpszClassName;
			wcx.lpszMenuName =  MAKEINTRESOURCE(nIDMenu);    
			wcx.style = dwStyle;                   
			wcx.hCursor = hCursor;
			wcx.hbrBackground = hbrBg; 
			wcx.hIcon = hIcon;
			wcx.lpfnWndProc = WndProc;     
		 
			if (!RegisterClassEx(&wcx))
				if (GetLastError() != ERROR_CLASS_ALREADY_EXISTS)
					throw Exception() << StrError(GetLastError());
		}

		void CreateEx(DWORD dwExStyle, HINSTANCE hInst, LPCTSTR lpszClassName, 
				    LPCTSTR lpszWindowName, DWORD dwStyle = WS_OVERLAPPEDWINDOW, 
				    int x = CW_USEDEFAULT, int y = 0, int nWidth = CW_USEDEFAULT,
				    int nHeight = 0, HWND pParentWnd = NULL, UINT nID = 0)
		{
			m_hWnd = CreateWindowEx(dwExStyle, lpszClassName, lpszWindowName, dwStyle,
				x, y, nWidth, nHeight, pParentWnd, reinterpret_cast<HMENU>(nID), hInst, this);

			if (m_hWnd == NULL)
				throw Exception() << StrError(GetLastError());
			
			WNDCLASSEX wcx;
			if (GetClassInfoEx(hInst, lpszClassName, &wcx))
			{
				if (wcx.lpfnWndProc != WndProc) // Subclassing ?
				{
					m_pPrevWndProc = wcx.lpfnWndProc;
					SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WndProc));
					SetWindowLongPtr(m_hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
				}
			}
		}

		void Create(HINSTANCE hInst, LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
					DWORD dwStyle = WS_OVERLAPPEDWINDOW, int x = CW_USEDEFAULT,
					int y = 0, int nWidth = CW_USEDEFAULT,
					int nHeight = 0, HWND pParentWnd = NULL, 
					UINT nID = 0)
		{
			CreateEx(0,  hInst, lpszClassName, lpszWindowName, dwStyle, 
				x, y, nWidth, nHeight, pParentWnd, nID);
		}

		void ShowWindow(int nCmdShow)
		{
			::ShowWindow(m_hWnd, nCmdShow);
			UpdateWindow(m_hWnd);
		}

		inline operator HWND(void) const {return m_hWnd;}
	};
}
