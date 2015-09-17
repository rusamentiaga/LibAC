#include "stdafx.h"

#include "Resource.h"
#include "../../Src/Window.h"
#include "../../Src/Dialog.h"
#include "../../Src/tstring.h"

const unsigned short MAX_NAME_LENGTH = 128;

class PersonalDataDialog : public AC::Dialog
{
protected:
	AC::tstring m_Name;
	unsigned short m_BirthYear;
	
	LRESULT OnWmInitdialog(WPARAM /*wParam*/, LPARAM /*lParam*/)
	{
		SetFocus(GetDlgItem(*this, IDC_NAME));
		return TRUE;
	}

	LRESULT OnOK(WPARAM /*wParam*/, LPARAM /*lParam*/)
	{
		BOOL bTranslated;
		TCHAR Buffer[MAX_NAME_LENGTH];

		GetDlgItemText(*this, IDC_NAME, Buffer, MAX_NAME_LENGTH);
		m_Name = Buffer;

		m_BirthYear = static_cast<unsigned short>
			(GetDlgItemInt(m_hWnd, IDC_BIRTH, &bTranslated, FALSE));
		if (!bTranslated)
		{
			MessageBox(*this, _T("Fecha incorrecta"), _T("Error"), MB_OK|MB_ICONERROR);
			return TRUE;
		}
		EndDialog(IDOK);
		return TRUE;
	}

	LRESULT OnCancel(WPARAM /*wParam*/, LPARAM /*lParam*/)
	{
		EndDialog(IDCANCEL);
		return TRUE;
	}

public:
	PersonalDataDialog() : Dialog(IDD_PERSONAL_DATA)
	{
		AddMessageHandler(WM_INITDIALOG, 
			static_cast<MsgHandler>(&PersonalDataDialog::OnWmInitdialog));
		AddMessageHandler(WM_COMMAND, 
			static_cast<MsgHandler>(&PersonalDataDialog::OnOK), 
						  IDOK, BN_CLICKED);
		AddMessageHandler(WM_COMMAND, 
			static_cast<MsgHandler>(&PersonalDataDialog::OnCancel), 
						  IDCANCEL, BN_CLICKED);
	}

	AC::tstring GetName() { return m_Name; }

	unsigned short GetBirthYear() { return m_BirthYear; }
};

class MainWindow : public AC::Window
{
protected:
	virtual LRESULT OnWmCreate(WPARAM /*wParam*/, LPARAM /*lParam*/)
	{
		return 0;
	}

	virtual LRESULT OnExit(WPARAM /*wParam*/, LPARAM /*lParam*/)
	{
		DestroyWindow();
		return 0;
	}

	virtual LRESULT OnWmDestroy(WPARAM /*wParam*/, LPARAM /*lParam*/)
	{
		PostQuitMessage(0);
		return 0;
	}

	virtual LRESULT OnFileInsertPersonalData(WPARAM /*wParam*/, LPARAM /*lParam*/)
	{
		PersonalDataDialog Dlg;
		if (Dlg.DoModal(*this, GetModuleHandle(NULL)) == IDOK)
		{
			AC::tstring Name = Dlg.GetName();
			unsigned short BirthYear = Dlg.GetBirthYear();

			AC::tostringstream oss;
			oss << _T("El usuario se llama ") << Name <<
				_T(" y ha nacido en el año ") << BirthYear;
			MessageBox(*this, oss.str().c_str(), _T("Datos personales"), 
				MB_OK|MB_ICONINFORMATION);
		}			
		return 0;
	}

public:
	MainWindow()
	{
		AddMessageHandler(WM_CREATE, 
			static_cast<MsgHandler>(&MainWindow::OnWmCreate));
		AddMessageHandler(WM_DESTROY, 
			static_cast<MsgHandler>(&MainWindow::OnWmDestroy));
		AddMessageHandler(WM_COMMAND, 
			static_cast<MsgHandler>(&MainWindow::OnFileInsertPersonalData), 
						  ID_FILE_INSERT_PD, BN_CLICKED);
		AddMessageHandler(WM_COMMAND, 
			static_cast<MsgHandler>(&MainWindow::OnExit), IDM_EXIT, BN_CLICKED);		
	}
};

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	try
	{
		LPCTSTR WindowClass = _T("SAMPLEWINDOW");
		LPCTSTR WindowTitle = _T("Sample-Window");

		MainWindow Window;
		Window.RegisterClass(hInstance, WindowClass, IDC_SAMPLEWINDOW);
		Window.Create(hInstance, WindowClass, WindowTitle);
		Window.ShowWindow(nCmdShow);

		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return (int)msg.wParam;
	}
	catch(std::exception& ex)
	{
		AC::tstring Reason = AnsiTotstring(ex.what());
		MessageBox(NULL, Reason.c_str(), _T("Error en la aplicación"), MB_OK|MB_ICONERROR);
		return EXIT_FAILURE;
	}
}


