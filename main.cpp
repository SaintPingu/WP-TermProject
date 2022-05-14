#pragma warning(disable:28251)
#include <Windows.h>
#include <gdiplus.h>
#include <time.h>
#include <atlImage.h>
#include <cassert>
#include "interface.h"

#define DEBUG_MODE false

void InitPaint(HWND hWnd, PAINTSTRUCT& ps, HDC& hdc, HDC& memDC, HBITMAP& hBitmap, RECT& rectWindow);
void ReleasePaint(HWND hWnd, PAINTSTRUCT& ps, HDC& hdc, HDC& memDC, HBITMAP& hBitmap, RECT& rectWindow);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HINSTANCE hInst;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	srand((unsigned int)time(NULL));

	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	hInst = hInstance;

	LPCTSTR lpszClass = L"Window Class Name";
	LPCTSTR lpszWindowName = L"windows program 2";

	static Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	static ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass,
		lpszWindowName,
		WS_OVERLAPPED | WS_SYSMENU,
		0,
		0,
		WINDOWSIZE_X,
		WINDOWSIZE_Y,
		NULL,
		(HMENU)NULL,
		hInstance,
		NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	Gdiplus::GdiplusShutdown(gdiplusToken);
	return Message.wParam;
}

FlyPokemon* flyPokemon;
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc, memDC;
	HBITMAP hBitmap;

	static RECT rectWindow;
	static ObjectImage moltres;
	static GameData gameData;

	switch (uMsg)
	{
	case WM_CREATE:
	{
		GetClientRect(hWnd, &rectWindow);
		moltres.Load(L"paint_moltres.png", { 85, 77 }, { 35, 25 }, { 15,35 });
		flyPokemon = new FlyPokemon(hWnd, moltres, 1.5f, 1.5f, { 300, 300 });

		SetTimer(hWnd, TIMERID_INVALIDATE, ELAPSE_INVALIDATE, T_Invalidate);
		SetTimer(hWnd, TIMERID_ANIMATION, ELAPSE_ANIMATION, T_Animate);
	}
	break;
	case WM_ERASEBKGND:
		return FALSE;
	case WM_PAINT:
	{
		InitPaint(hWnd, ps, hdc, memDC, hBitmap, rectWindow);
		flyPokemon->Paint(memDC);
		ReleasePaint(hWnd, ps, hdc, memDC, hBitmap, rectWindow);
	}
	break;
	case WM_KEYDOWN:
		CheckKeyDown(hWnd, wParam, &gameData, flyPokemon);
		break;
	case WM_KEYUP:
		CheckKeyUp(hWnd, wParam, &gameData, flyPokemon);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void InitPaint(HWND hWnd, PAINTSTRUCT& ps, HDC& hdc, HDC& memDC, HBITMAP& hBitmap, RECT& rectWindow)
{
	hdc = BeginPaint(hWnd, &ps);
	memDC = CreateCompatibleDC(hdc);
	hBitmap = CreateCompatibleBitmap(hdc, rectWindow.right, rectWindow.bottom);
	SelectObject(memDC, hBitmap);
	SelectObject(memDC, GetStockObject(WHITE_BRUSH));
	Rectangle(memDC, 0, 0, rectWindow.right, rectWindow.bottom);
	SetStretchBltMode(hdc, COLORONCOLOR);
	SetStretchBltMode(memDC, COLORONCOLOR);
}
void ReleasePaint(HWND hWnd, PAINTSTRUCT& ps, HDC& hdc, HDC& memDC, HBITMAP& hBitmap, RECT& rectWindow)
{
	BitBlt(hdc, 0, 0, rectWindow.right, rectWindow.bottom, memDC, 0, 0, SRCCOPY);
	ValidateRect(hWnd, NULL);
	DeleteDC(memDC);
	DeleteObject(hBitmap);
	EndPaint(hWnd, &ps);
}