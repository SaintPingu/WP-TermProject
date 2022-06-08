#pragma once

class SceneManager {
private:
	Scene crntScene = Scene::Title;

	RECT rectWindow = { 0, };
	RECT rectDisplay = { 0, };
	CImage bkground{};

	void StartPaint(const HWND& hWnd, PAINTSTRUCT& ps, HDC& hdc, HDC& memDC, HBITMAP& hBitmap, RECT& rectWindow) const;
	void FinishPaint(const HWND& hWnd, PAINTSTRUCT& ps, HDC& hdc, HDC& memDC, HBITMAP& hBitmap, RECT& rectWindow) const;
	void DeleteScene(const HWND& hWnd);
	void LoadScene(const HWND& hWnd);

public:
	void Init(const HWND& hWnd);
	void Paint(const HWND& hWnd);
	void MoveScene(const HWND& hWnd, Scene scene);


	inline constexpr RECT GetRectDisplay() const
	{
		return rectDisplay;
	}
	inline constexpr Scene GetScene()
	{
		return crntScene;
	}
};