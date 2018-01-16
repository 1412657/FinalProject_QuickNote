/*---------------------------------------
| Student ID : 1412657				|
| Full Name : Nguyễn Tường Vy   	|
| Email : vytuong1120@gmail.com		|
-----------------------------------------
Edit on

Environment: 	Windows 10
Tool : Visual Studio 2017
*/

// QuickNote.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "QuickNote.h"
#include <commctrl.h>
#include <string>     // std::wstring, std::to_wstring
#include <windowsX.h>
#include <cstring>
#include <Shellapi.h>
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "ComCtl32.lib")

#define MAX_LOADSTRING 100
#define QUICKNOTEHOTKEYID 1
using namespace std;

// Global Variables:
HINSTANCE g_HInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
WCHAR szTagFilePath[MAX_LOADSTRING + 21];
WCHAR szNoteFilePath[MAX_LOADSTRING + 21];
const unsigned short MAX_HANDLENOTENUM = 100;
const unsigned short MAX_HANDLEDTAGNUM = 100;

const unsigned short MAX_TAGNUM = 24;
const unsigned short MAX_OPENEDNOTE = 10;
CNote * g_OpendNote[MAX_OPENEDNOTE];
HWND btn_tag_hwndList[MAX_TAGNUM];
HFONT g_HFont;
HFONT g_NoteHFont;
//HMENU hMenu;
NOTIFYICONDATA NID;
BOOL minimized = false;
HWND g_HWnd, g_btnAddTagMain, g_tbAddTagMain;
HWND g_NoteHwnd[MAX_OPENEDNOTE];
WNDCLASSEXW g_NoteWC[MAX_OPENEDNOTE];
HWND tb_noteEdit[MAX_OPENEDNOTE];
HWND btn_noteAddTag[MAX_OPENEDNOTE];
LPWSTR noteClsName[MAX_OPENEDNOTE];
short g_OpenedNoteNum = 0;
bool _isDragging = false;
vector<CNote> noteList;
short curViewedTag;
vector<int> mapping;

RECT MainRect;
POINT point;
POINT curpoint;

DWORD color[14] = { 0xf0f0f0,
					0x0000c0,
					0x00c0c0,
					0x00c000,
					0xc0c000,
					0xc00000,
					0xc000c0,
					0x606060,
					0x6060c0,
					0x0060c0,
					0xc06060,
					0x60c060,
					0xc060c0,
					0xc0c0c0 };


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	WndProcNote(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    ViewNote(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	AddTag(HWND, UINT, WPARAM, LPARAM);

BOOL OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct);
void OnNoteSize(HWND hwnd, UINT state, int cx, int cy);

void OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify);
void OnGetMinMaxInfo(HWND hwnd, LPMINMAXINFO lpMinMaxInfo);
void OnGetNoteMinMaxInfo(HWND hwnd, LPMINMAXINFO lpMinMaxInfo);
void OnNoteCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify);
LRESULT OnNotify(HWND hwnd, int idFrom, NMHDR *pnm);

int AddTagButton(unsigned short tagIndex, LPWSTR tagName);
void HotKeyProc();
short GetHWndIndex(HWND hWnd);
void LoadTag();
void LoadTagBin();
void SaveTag();
void SaveTagBin();
void LoadNoteBin();
void SaveNoteBin();


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	if (MAX_TAGNUM > MAX_HANDLEDTAGNUM)
	{
		MessageBox(nullptr, L"Program is unable to handle such many tags", L"Error", 0);
		return -1;
	}
	if (MAX_OPENEDNOTE > MAX_HANDLENOTENUM)
	{
		MessageBox(nullptr, L"Program is unable to handle such many opened notes", L"Error", 0);
		return -1;
	}
	for (int i = 0; i < MAX_OPENEDNOTE; ++i)
	{
		g_NoteHwnd[i] = 0;
		g_OpendNote[i] = 0;
	}

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_MY1412657, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);



	if (RegisterHotKey(
		g_HWnd,
		QUICKNOTEHOTKEYID,
		MOD_CONTROL | MOD_ALT | MOD_SHIFT,
		'N'))
	{
		ShowWindow(g_HWnd, SW_RESTORE);
		MessageBox(g_HWnd, L"Hotkey 'CTRL + ALT + SHIFT + N' registered.\n", L"Warning", 0);
		BringWindowToTop(g_HWnd);
		ShowWindow(g_HWnd, SW_MINIMIZE);
	}
	else
	{
		ShowWindow(g_HWnd, SW_RESTORE);
		MessageBox(g_HWnd, L"Hotkey 'CTRL + ALT + SHIFT + N' did not register.\n", L"Error", 0);
		BringWindowToTop(g_HWnd);
		ShowWindow(g_HWnd, SW_MINIMIZE);
	}
	// Perform application initialization:
	if (!InitInstance(hInstance, SW_MINIMIZE))
	{
		return FALSE;
	}
	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1412657));
	PostMessage(g_HWnd, WM_SIZE, SIZE_MINIMIZED, 0);




	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (msg.message == WM_HOTKEY && LOWORD(msg.wParam) == QUICKNOTEHOTKEYID)
		{
			HotKeyProc();

		}


		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY1412657));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MY1412657);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}
//
//  FUNCTION: MyRegisterNoteClass()
//
//  PURPOSE: Registers the note window class.
//
ATOM MyRegisterNoteClass(HINSTANCE hInstance, int index)
{
	if (index == MAX_OPENEDNOTE)
		return -1;

	g_NoteWC[index] = { 0 };
	g_NoteWC[index].cbClsExtra = 0;
	g_NoteWC[index].cbWndExtra = 0;
	g_NoteWC[index].hInstance = g_HInst;
	g_NoteWC[index].lpfnWndProc = WndProcNote;
	g_NoteWC[index].lpszClassName = szWindowClass;
	g_NoteWC[index].hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	g_NoteWC[index].hIcon = LoadIcon(g_HInst, MAKEINTRESOURCE(IDC_MY1412657));
	g_NoteWC[index].hIconSm = LoadIcon(g_HInst, MAKEINTRESOURCE(IDC_MY1412657));
	g_NoteWC[index].hCursor = LoadCursor(nullptr, IDC_ARROW);

	ATOM ret = RegisterClassExW(g_NoteWC + index);


	return ret;
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	g_HInst = hInstance; // Store instance handle in our global variable

	g_HWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	//LONG style = GetWindowLong(hWnd, GWL_STYLE);
	//style &= ~(WS_BORDER | WS_CAPTION /*| WS_THICKFRAME*/);
	//SetWindowLong(hWnd, GWL_STYLE, WS_BORDER | WS_POPUP | WS_THICKFRAME);
	SetMenu(g_HWnd, nullptr);

	SetWindowPos(g_HWnd, NULL, 0, 0, 0, 0,
		SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE
		| SWP_FRAMECHANGED);
	NID.cbSize = sizeof(NID);
	NID.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY1412657));
	NID.uCallbackMessage = WM_USER + 1;
	NID.hWnd = g_HWnd;
	NID.uID = WM_USER + 2;
	wcscpy_s(NID.szTip, L"Quick Note Tray Icon");
	NID.uFlags = NIF_TIP | NIF_ICON | NIF_MESSAGE;

	//setup for tag info file
	wsprintf(szTagFilePath, L"tags");

	//setup for note info file
	wsprintf(szNoteFilePath, L"notes");

	// tag list loading
	ZeroMemory(btn_tag_hwndList, MAX_TAGNUM * sizeof(btn_tag_hwndList[0]));

	LoadTagBin();
	LoadNoteBin();

	g_btnAddTagMain = CreateWindow(L"BUTTON", L"Add tag", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP, 330, 10, 50, 23, g_HWnd, (HMENU)(IDC_BUTTON_ADDTAGMAIN), g_HInst, 0);
	SendMessage(g_btnAddTagMain, WM_SETFONT, (WPARAM)g_HFont, TRUE);

	g_tbAddTagMain = CreateWindow(WC_EDIT, L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL, 10, 12, 310, 19, g_HWnd, (HMENU)IDC_EDIT_ADDTAGMAIN, 0, 0);
	SendMessage(g_tbAddTagMain, WM_SETFONT, (WPARAM)g_HFont, TRUE);


	if (!g_HWnd)
	{
		return FALSE;
	}

	ShowWindow(g_HWnd, nCmdShow);
	UpdateWindow(g_HWnd);

	return TRUE;
}

BOOL InitNoteInstance(HINSTANCE hInstance, int nCmdShow, int index)
{
	g_NoteHwnd[index] = CreateWindow(g_NoteWC[index].lpszClassName, 0, WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_EX_TOOLWINDOW | WS_POPUPWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, NULL, NULL, hInstance, NULL);
	//LONG style = GetWindowLong(g_NoteHwnd[index], GWL_STYLE);
	//style &= ~(WS_BORDER | WS_CAPTION /*| WS_THICKFRAME*/);
	//SetWindowLong(g_NoteHwnd[index], GWL_STYLE, style);

	SetMenu(g_NoteHwnd[index], nullptr);
	++g_OpenedNoteNum;

	if (!g_NoteHwnd[index])
	{
		return FALSE;
	}


	tb_noteEdit[index] = CreateWindow(WC_EDIT, L"", WS_CHILD | WS_VISIBLE /*| WS_BORDER*/ | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
		0, 30, 80, 80, g_NoteHwnd[index], (HMENU)IDC_BUTTON_NOTE0 + index, 0, 0);
	SendMessage(tb_noteEdit[index], WM_SETFONT, (WPARAM)g_NoteHFont, TRUE);
	SetWindowPos(g_NoteHwnd[index], 0, CW_USEDEFAULT, CW_USEDEFAULT, 400, 400, SWP_NOMOVE);

	btn_noteAddTag[index] = CreateWindow(WC_BUTTON, L"Add Tag", WS_CHILD | WS_VISIBLE /*| WS_BORDER */ | BS_PUSHBUTTON,
		0, 0, 70, 30, g_NoteHwnd[index], (HMENU)IDC_BUTTON_ADDTAG0 + index, 0, 0);
	SendMessage(btn_noteAddTag[index], WM_SETFONT, (WPARAM)g_HFont, TRUE);


	SetWindowPos(g_NoteHwnd[index], 0, CW_USEDEFAULT, CW_USEDEFAULT, 400, 400, SWP_NOMOVE);
	ShowWindow(g_NoteHwnd[index], nCmdShow);
	UpdateWindow(g_NoteHwnd[index]);
	BringWindowToTop(g_NoteHwnd[index]);


	return true;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (GetHWndIndex(hWnd) != -1)
	{

		return WndProcNote(hWnd, message, wParam, lParam);;
	}
	switch (message)
	{
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
		HANDLE_MSG(hWnd, WM_GETMINMAXINFO, OnGetMinMaxInfo);
	case WM_SIZE:
	{
		if (wParam == SIZE_MINIMIZED)
		{
			minimized = true;
			ShowWindow(hWnd, SW_HIDE);
			Shell_NotifyIcon(NIM_ADD, &NID);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...


		int tagNum = CTagManager::GetInstance()->GetTagNum();
		int totalTaggingNum = 0;
		int *taggingNum = new int[tagNum];
		for (int i = 0; i < tagNum; ++i)
			taggingNum[i] = 0;

		for (unsigned int i = 0; i < noteList.size(); ++i)
		{
			for (int j = 0; j < noteList[i].GetTagNum(); ++j)
			{
				taggingNum[CTagManager::GetInstance()->GetTagIndex(noteList[i].GetTag(j))] ++;
				++totalTaggingNum;
			}
		}

		HBRUSH hbrush, hbrOld;
		HPEN hpen, hpenOld;

		int left = 10;
		for (int i = 0; i < tagNum; ++i)
		{
			hbrush = CreateSolidBrush(NULL_BRUSH);
			hbrOld = (HBRUSH)SelectObject(hdc, hbrush);
			hpen = CreatePen(PS_SOLID, 0, color[i]);
			hpenOld = (HPEN)SelectObject(hdc, hpen);
			Rectangle(hdc, 10 + (i % 3) * 125 - 1, 76 + (i / 3) * 28 - 1, 10 + (i % 3) * 125 + 120 + 1, 76 + (i / 3) * 28 + 23 + 1);
			DeleteObject(hbrush);
			DeleteObject(hpen);


			hbrush = CreateSolidBrush(color[i]);
			SelectObject(hdc, hbrush);
			hpen = CreatePen(NULL_PEN, 0, color[i]);
			SelectObject(hdc, hpen);
			if (totalTaggingNum != 0)
			{
				Rectangle(hdc, left, 41, int(left + 1.0 * taggingNum[i] / totalTaggingNum * 370), 66);
				left = int(left + 1.0*taggingNum[i] / totalTaggingNum * 370);
			}

			DeleteObject(hbrush);
			DeleteObject(hpen);

			SelectObject(hdc, hbrOld);
			SelectObject(hdc, hpenOld);

		}

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		if (hWnd != g_HWnd)
			break;
		UnregisterHotKey(hWnd, QUICKNOTEHOTKEYID);
		SaveTagBin();
		SaveNoteBin();
		PostQuitMessage(0);

		break;
	default:
		if (message == NID.uCallbackMessage)
		{
			switch (lParam)
			{
			case WM_RBUTTONDOWN:
			{
				HMENU popMenu = CreatePopupMenu();
				AppendMenu(popMenu, MF_STRING, ID_MENU_SHOW, L"Show");
				AppendMenu(popMenu, MF_STRING, ID_MENU_VIEW_NOTES, L"View notes");
				AppendMenu(popMenu, MF_STRING, ID_MENU_EXIT, L"Exit");
				POINT pCursor;
				GetCursorPos(&pCursor);
				TrackPopupMenu(popMenu, TPM_LEFTBUTTON | TPM_RIGHTALIGN, pCursor.x, pCursor.y, 0, hWnd, NULL);
			}
			break;
			case WM_LBUTTONUP: //Did the user left click?
			{ //Show the window and remove the systray icon.

				ShowWindow(hWnd, SW_SHOW);

				if (minimized)
					ShowWindow(hWnd, SW_RESTORE);
				BringWindowToTop(hWnd);


				Shell_NotifyIcon(NIM_DELETE, &NID);


			}
			break;
			}
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the note window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProcNote(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	LRESULT Result = 0;

	switch (Msg)
	{
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hWnd, WM_COMMAND, OnNoteCommand);
		//HANDLE_MSG(hWnd, WMg_ETMINMAXINFO, OnGetMinMaxInfo);
		HANDLE_MSG(hWnd, WM_SIZE, OnNoteSize);

		HANDLE_MSG(hWnd, WM_GETMINMAXINFO, OnGetNoteMinMaxInfo);
	case WM_LBUTTONDOWN:
	{


		if (!_isDragging)
		{
			SetCapture(hWnd);
			GetWindowRect(hWnd, &MainRect);
			//save current cursor coordinate
			GetCursorPos(&point);
			ScreenToClient(hWnd, &point);
			break;
		}
		break;
	}
	case WM_MOUSEMOVE:
	{
		if (_isDragging)
		{
			GetCursorPos(&curpoint);
			if (wParam == MK_LBUTTON)
			{
				MoveWindow(hWnd, curpoint.x - point.x, curpoint.y - point.y,
					MainRect.right - MainRect.left, MainRect.bottom - MainRect.top,
					TRUE);
			}
			break;

		}
		break;
	}
	case WM_LBUTTONUP:
	{
		ReleaseCapture();
		_isDragging = false;
		break;
	}

	case WM_CLOSE:
	{
		int index = GetHWndIndex(hWnd);
		if (hWnd != g_NoteHwnd[index])
			return 1;
		--g_OpenedNoteNum;
		WCHAR buffer[CNote::MAXLENGTH];
		ZeroMemory(buffer, CNote::MAXLENGTH * sizeof(WCHAR));
		if (!(wcslen(buffer) == 0 && g_OpendNote[index]->GetTagNum() == 0))
		{
			GetWindowText(tb_noteEdit[index], buffer, CNote::MAXLENGTH);
			g_OpendNote[index]->Replace(buffer);
			noteList.push_back(*g_OpendNote[index]);
		}
		delete g_OpendNote[index];
		g_NoteHwnd[index];
		DestroyWindow(hWnd);
		InvalidateRect(g_HWnd, 0, 0);
		//PostQuitMessage(0);

		return 0;
		break;
	}
	case WM_DESTROY:
	{
		DestroyWindow(hWnd);
		//PostQuitMessage(0);

		return 0;
		break;
	}
	default:
		Result = DefWindowProc(hWnd, Msg, wParam, lParam);
	}

	return(Result);
}

BOOL OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
{
	//if (hWnd != g_HWnd)
	//	return false;
	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof(icc);
	icc.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icc);

	// Lấy font hệ thống
	LOGFONT lf;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	g_HFont = CreateFont(lf.lfHeight, lf.lfWidth,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);
	g_NoteHFont = CreateFont(lf.lfHeight - 13, lf.lfWidth - 13,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, L"UVN Bay Buom");


	/*btn_tag_hwndList[tagIndex] = */






	return true;
}


void OnNoteSize(HWND hWnd, UINT state, int cx, int cy)
{
	int i = GetHWndIndex(hWnd);
	SetWindowPos(tb_noteEdit[i], 0, 0, 30, cx, cy - 30, SWP_NOMOVE);

}

void OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
{
	if (1101 <= id && id >= 1024)
	{
		curViewedTag = id - 1101;
		DialogBox(g_HInst, MAKEINTRESOURCE(IDD_VIEWNOTELIST), hWnd, ViewNote);
	}
	switch (id)
	{
		// Parse the popup menu selections:
	case ID_MENU_SHOW:
		if (minimized)
			ShowWindow(hWnd, SW_RESTORE);
		BringWindowToTop(hWnd);
		break;
	case ID_MENU_VIEW_NOTES:
	{
		curViewedTag = -1;
		if (minimized)
			ShowWindow(hWnd, SW_RESTORE);
		BringWindowToTop(hWnd);
		DialogBox(g_HInst, MAKEINTRESOURCE(IDD_VIEWNOTELIST), g_HWnd, ViewNote);

	}

	case ID_MENU_EXIT:
		UnregisterHotKey(hWnd, QUICKNOTEHOTKEYID);
		SaveTagBin();
		SaveNoteBin();
		PostQuitMessage(0);
		break;
	case IDC_BUTTON_ADDTAGMAIN:
	{
		WCHAR buff[CTag::MAX_TAGNAMELENGHTH + 1];
		GetWindowText(g_tbAddTagMain, buff, CTag::MAX_TAGNAMELENGHTH + 1);
		if (buff[0] == 0)
			break;
		SetWindowText(g_tbAddTagMain, L"");
		AddTagButton(CTagManager::GetInstance()->GetTagNum(), buff);
		break;
	}
	case IDM_ABOUT:
		DialogBox(g_HInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		break;
	case IDM_EXIT:
		UnregisterHotKey(hWnd, QUICKNOTEHOTKEYID);
		SaveTagBin();
		SaveNoteBin();
		DestroyWindow(hWnd);
		break;
	}
}

void OnNoteCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
{
	if (300 <= id && id < 300 + MAX_OPENEDNOTE)
	{
		//add tag for a note
		int index = GetHWndIndex(hWnd);
		DialogBox(g_HInst, MAKEINTRESOURCE(IDD_ADDTAG), hWnd, AddTag);



	}
	//switch (id)
	//{
	//	
	//}
}

void OnGetMinMaxInfo(HWND hwnd, LPMINMAXINFO lpMinMaxInfo)
{
	lpMinMaxInfo->ptMinTrackSize.x = 405;
	lpMinMaxInfo->ptMaxTrackSize.x = 405;
	lpMinMaxInfo->ptMinTrackSize.y = 550;
	//lpMinMaxInfo->ptMaxTrackSize.x = 1000;
}
void OnGetNoteMinMaxInfo(HWND hwnd, LPMINMAXINFO lpMinMaxInfo)
{
	lpMinMaxInfo->ptMinTrackSize.x = 300;
	lpMinMaxInfo->ptMinTrackSize.y = 300;
	//lpMinMaxInfo->ptMinTrackSize.y = 550;
	//lpMinMaxInfo->ptMaxTrackSize.x = 1000;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK ViewNote(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		bool filter = (curViewedTag == -1 ? true : false);
		CTag * pTag = CTagManager::GetInstance()->GetTag(curViewedTag);
		HWND hList = GetDlgItem(hDlg, IDC_LIST1);
		SetWindowLong(hList, GWL_STYLE, WS_VISIBLE | WS_CHILD | LVS_REPORT | WS_VSCROLL | WS_HSCROLL | LVS_SINGLESEL);
		LVCOLUMN lvCol;
		RECT rect;
		GetWindowRect(hList, &rect);
		int width = rect.right - rect.left;

		// 1st column
		lvCol.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
		lvCol.fmt = LVCFMT_LEFT;
		lvCol.cx = width / 4;
		lvCol.pszText = L"Note name";
		ListView_InsertColumn(hList, 0, &lvCol);

		// 2nd column
		lvCol.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
		lvCol.fmt = LVCFMT_RIGHT;
		lvCol.cx = width / 2;
		lvCol.pszText = L"Content";
		ListView_InsertColumn(hList, 1, &lvCol);

		// 3rd column
		lvCol.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
		lvCol.fmt = LVCFMT_RIGHT;
		lvCol.cx = int(width / 4.2);
		lvCol.pszText = _T("Number of tags");
		ListView_InsertColumn(hList, 2, &lvCol);

		LVITEM insrtItem;
		WCHAR buff[20];
		WCHAR content[CNote::BRIEFLENGTH + 1];
		int item = 0;
		for (unsigned int i = 0; i < noteList.size(); ++i)
		{
			if (filter || noteList[i].IsExist(pTag->tagName))
			{

				wsprintf(buff, L"Note %d", item + 1);

				insrtItem.mask = LVIF_TEXT;
				insrtItem.iItem = item;
				insrtItem.pszText = buff;
				insrtItem.cchTextMax = 256;
				insrtItem.iSubItem = 0;
				ListView_InsertItem(hList, &insrtItem);
				wcscpy_s(content, CNote::BRIEFLENGTH, (LPWSTR)noteList[i].GetBrieftContent().c_str());
				ListView_SetItemText(hList, item, 1, content);
				wsprintf(buff, L"%d", noteList[i].GetTagNum());
				ListView_SetItemText(hList, item, 2, buff);
				mapping.push_back(i);

				++item;
			}
		}

	}
	return (INT_PTR)TRUE;

	HANDLE_MSG(hDlg, WM_NOTIFY, OnNotify);

	case WM_COMMAND:
		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
LRESULT OnNotify(HWND hwnd, int idFrom, NMHDR *pnm)
{
	HWND hList = GetDlgItem(hwnd, IDC_LIST1);
	switch (pnm->code)
	{

	case NM_DBLCLK:
	{
		if (pnm->hwndFrom == hList)
		{
			int index = SendMessage(hList, LVM_GETNEXTITEM, (WPARAM)-1, (LPARAM)LVNI_SELECTED);
			if (index < 0)
				return 0;
			int noteIndex = mapping[index];
			CNote note = noteList[noteIndex];
			noteList.erase(noteList.begin() + index);


			if (g_OpenedNoteNum == MAX_OPENEDNOTE)
			{
				MessageBox(g_HWnd, L"You are opening so many note. Please close some notes first.", L"Limit Exceeded", 0);

				return 0;
			}
			index = 0;
			for (; index < MAX_OPENEDNOTE; ++index)
			{
				if (g_NoteHwnd[index] == 0)
					break;
			}
			if (index == MAX_OPENEDNOTE)
				return 0;
			MyRegisterNoteClass(g_HInst, index);
			InitNoteInstance(g_HInst, 10, index);
			if (g_OpendNote[index])
				delete g_OpendNote[index];
			g_OpendNote[index] = new CNote(note);
			g_OpendNote[index]->Replace(note.GetContent());
			SetWindowText(tb_noteEdit[index], g_OpendNote[index]->GetContent().c_str());
			mapping[index] = noteList.size() - 1;
		}
	}
	break;
	}
	return 0;
}
INT_PTR CALLBACK AddTag(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int index = GetHWndIndex(GetParent(hDlg));
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		int tagNum = CTagManager::GetInstance()->GetTagNum();
		for (int i = 0, j = 0; i < tagNum; ++i)
		{
			HWND hWnd = GetDlgItem(hDlg, IDC_CHKTAG1 + i);
			SetWindowText(hWnd, CTagManager::GetInstance()->GetTag(i)->tagName);
		}
		for (int i = tagNum; i < MAX_TAGNUM; ++i)
		{
			HWND hWnd = GetDlgItem(hDlg, IDC_CHKTAG1 + i);
			ShowWindow(hWnd, SW_HIDE);
		}
		for (int i = 0; i < g_OpendNote[index]->GetTagNum(); ++i)
		{
			CTag *ptag = g_OpendNote[index]->GetTag(i);
			HWND hWnd = GetDlgItem(hDlg, IDC_CHKTAG1 + CTagManager::GetInstance()->GetTagIndex(ptag));
			SendMessage(hWnd, BM_SETCHECK, BST_CHECKED, 0);
		}
		return (INT_PTR)TRUE;
	}
	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDOK)
		{
			g_OpendNote[index]->RemoveAllTag();
			int tagNum = CTagManager::GetInstance()->GetTagNum();
			for (int i = 0, j = 0; i < tagNum; ++i)
			{
				HWND hWnd = GetDlgItem(hDlg, IDC_CHKTAG1 + i);
				if (SendMessage(hWnd, BM_GETCHECK, 0, 0) == BST_CHECKED)
				{
					WCHAR tagName[CTag::MAX_TAGNAMELENGHTH + 1];
					GetWindowText(hWnd, tagName, CTag::MAX_TAGNAMELENGHTH + 1);
					g_OpendNote[index]->AddTagWithCtreateTag(tagName);
				}

			}
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		if (LOWORD(wParam) == IDCANCEL)
		{
			//do nothing
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
	}
	break;
	return (INT_PTR)FALSE;
	}
	return (INT_PTR)FALSE;
}
int AddTagButton(unsigned short tagIndex, LPWSTR tagName)
{
	if (tagIndex >= MAX_TAGNUM)
	{
		MessageBox(g_HWnd, L"Only 24 tags are allowed in max.", L"Limit Exceeded", 0);
		return -1;
	}
	CTag * lpTag;

	int ret = CTagManager::GetInstance()->AddTag(tagName, lpTag);
	if (ret != 0)
		return ret;

	btn_tag_hwndList[tagIndex] = CreateWindow(L"BUTTON", lpTag->tagShortName, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP, 10 + (tagIndex % 3) * 125, 76 + (tagIndex / 3) * 28, 120, 23, g_HWnd, (HMENU)(IDC_TAG1 + tagIndex), g_HInst, lpTag);
	SendMessage(btn_tag_hwndList[tagIndex], WM_SETFONT, (WPARAM)g_HFont, TRUE);
	InvalidateRect(g_HWnd, 0, 0);
	return 0;
}

void HotKeyProc()
{
	if (g_OpenedNoteNum == MAX_OPENEDNOTE)
	{
		MessageBox(g_HWnd, L"You are opening so many note. Please close some notes first.", L"Limit Exceeded", 0);

		return;
	}
	int index = 0;
	for (; index < MAX_OPENEDNOTE; ++index)
	{
		if (g_NoteHwnd[index] == 0)
			break;
	}
	if (index == MAX_OPENEDNOTE)
		return;
	MyRegisterNoteClass(g_HInst, index);
	InitNoteInstance(g_HInst, 10, index);
	if (g_OpendNote[index])
		delete g_OpendNote[index];
	g_OpendNote[index] = new CNote();
	SetWindowText(tb_noteEdit[index], g_OpendNote[index]->GetContent().c_str());

}

short GetHWndIndex(HWND hWnd)
{
	for (int i = 0; i < MAX_OPENEDNOTE; ++i)
	{
		if (hWnd == g_NoteHwnd[i])
			return i;
	}
	return -1;
}
void LoadTag()
{
	unsigned short i = 0;
	WCHAR buffer[MAX_LOADSTRING];
	FILE *stream = NULL;
	if (_wfopen_s(&stream, szTagFilePath, L"r") != ENOENT)
	{
		int i = 0;
		_wfopen_s(&stream, szTagFilePath, L"r, ccs=UTF-16LE");
		while (fgetws(buffer, MAX_LOADSTRING, stream) != NULL)
		{
			if (buffer[wcslen(buffer) - 1] == L'\n')
				buffer[wcslen(buffer) - 1] = 0;
			else
				buffer[wcslen(buffer)] = 0;

			if (AddTagButton(i, buffer) == -1)
				break;
			i++;
		}
		fclose(stream);

	}

}

void LoadTagBin()
{
	unsigned short i = 0;
	FILE *stream = NULL;
	WCHAR buff[CTag::MAX_TAGNAMELENGHTH + 1];
	short size = 0;
	int tagNum;

	if (_wfopen_s(&stream, szTagFilePath, L"rb") != ENOENT)
	{
		fread(&tagNum, sizeof(tagNum), 1, stream);
		fread(&size, sizeof(size), 1, stream);
		for (int i = 0; i < tagNum; ++i)
		{
			fread(buff, size, 1, stream);
			AddTagButton(i, buff); ;

		}

	}
	if (stream != NULL)
		fclose(stream);
}
void SaveTag()
{
	FILE *stream = NULL;
	int i = 0;
	int a = _wfopen_s(&stream, szTagFilePath, L"w,ccs=UTF-16LE");
	CTag *pTag = 0;
	int tagnum = CTagManager::GetInstance()->GetTagNum();
	while ((pTag = CTagManager::GetInstance()->GetTag(i)) != nullptr)
	{
		int a = fputws(pTag->tagName, stream);
		fputws(L"\n", stream);

		++i;
	}
	if (stream != NULL)
		fclose(stream);

}
void SaveTagBin()
{
	FILE *stream = NULL;
	_wfopen_s(&stream, szTagFilePath, L"wb");
	CTag *pTag = 0;
	short size = 0;
	int i = 0;
	int tagNum = CTagManager::GetInstance()->GetTagNum();
	fwrite((void *)&tagNum, sizeof(tagNum), 1, stream);
	size = sizeof(WCHAR) * (CTag::MAX_TAGNAMELENGHTH + 1);
	fwrite((void *)&size, sizeof(size), 1, stream);

	while ((pTag = CTagManager::GetInstance()->GetTag(i)) != nullptr)
	{
		fwrite((void *)pTag->tagName, size, 1, stream);

		++i;
	}
	if (stream != NULL)
		fclose(stream);
}

void LoadNoteBin()
{
	FILE *stream = NULL;
	if (_wfopen_s(&stream, szNoteFilePath, L"rb") == ENOENT)
		return;


	int noteNum;
	fread((void *)&noteNum, sizeof(noteNum), 1, stream);

	int contentLength = 0;
	wstring content;
	int tagNum;
	int tagIndex;
	CTag * pTag = 0;
	wchar_t * buffer = 0;

	for (int i = 0; i < noteNum; ++i)
	{
		CNote note;
		fread((void *)&contentLength, sizeof(contentLength), 1, stream);
		buffer = new wchar_t[contentLength + 1];
		fread((void *)buffer, sizeof(wchar_t), contentLength + 1, stream);
		note.Replace(buffer);

		fread((void *)&tagNum, sizeof(tagNum), 1, stream);
		for (int i = 0; i < tagNum; ++i)
		{
			fread((void *)&tagIndex, sizeof(tagIndex), 1, stream);
			note.AddTagWithoutCtreateTag(CTagManager::GetInstance()->GetTag(tagIndex)->tagName);
			//CTagManager::GetInstance()->GetTag(tagIndex)->RemoveRegistration();
		}

		delete[] buffer;
		noteList.push_back(note);
	}
	if (stream != NULL)
		fclose(stream);
}

void SaveNoteBin()
{
	int contentLength = 0;
	wstring content;
	int tagNum;
	int tagIndex;
	CTag * pTag = 0;
	int noteNum = noteList.size();
	FILE *stream = NULL;
	_wfopen_s(&stream, szNoteFilePath, L"wb");


	fwrite((void *)&noteNum, sizeof(noteNum), 1, stream);

	for (int i = 0; i < noteNum; ++i)
	{
		content = noteList[i].GetContent();
		contentLength = content.length();
		fwrite((void *)&contentLength, sizeof(contentLength), 1, stream);
		fwrite((void *)content.c_str(), sizeof(wchar_t), contentLength + 1, stream);
		tagNum = noteList[i].GetTagNum();
		fwrite((void *)&tagNum, sizeof(tagNum), 1, stream);
		for (int j = 0; j < tagNum; ++j)
		{
			pTag = noteList[i].GetTag(j);
			tagIndex = CTagManager::GetInstance()->GetTagIndex(pTag);
			fwrite((void *)&tagIndex, sizeof(tagIndex), 1, stream);
		}
	}
	if (stream != NULL)
		fclose(stream);
}