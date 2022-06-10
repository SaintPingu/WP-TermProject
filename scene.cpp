#include "stdafx.h"
#include "scene.h"
#include "interface.h"
#include "player.h"
#include "enemy.h"
#include "effect.h"
#include "boss.h"
#include "timer.h"
#include "sound.h"

extern GameData gameData;
extern Player* player;
extern EnemyController* enemies;
extern EffectManager* effects;
extern GUIManager* gui;
extern Boss* boss;
extern SoundManager* soundManager;

void SceneManager::StartPaint(const HWND& hWnd, PAINTSTRUCT& ps, HDC& hdc, HDC& memDC, HBITMAP& hBitmap, RECT& rectWindow) const
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
void SceneManager::FinishPaint(const HWND& hWnd, PAINTSTRUCT& ps, HDC& hdc, HDC& memDC, HBITMAP& hBitmap, RECT& rectWindow) const
{
	BitBlt(hdc, 0, 0, rectWindow.right, rectWindow.bottom, memDC, 0, 0, SRCCOPY);
	ValidateRect(hWnd, NULL);
	DeleteDC(memDC);
	DeleteObject(hBitmap);
	EndPaint(hWnd, &ps);
}

void SceneManager::DeleteScene(const HWND& hWnd)
{
	switch (crntScene)
	{
	case Scene::Battle:
		delete player;
		delete enemies;
		delete effects;
		delete boss;
		delete gui;

		player = nullptr;
		enemies = nullptr;
		effects = nullptr;
		boss = nullptr;
		gui = nullptr;

		KillTimer(hWnd, TIMERID_BATTLE_INVALIDATE);
		KillTimer(hWnd, TIMERID_BATTLE_ANIMATION);
		KillTimer(hWnd, TIMERID_BATTLE_EFFECT);
		KillTimer(hWnd, TIMERID_BATTLE_GUI);
		KillTimer(hWnd, TIMERID_BATTLE_ANIMATION_BOSS);
		break;
	}
}
void SceneManager::LoadScene(const HWND& hWnd)
{
	switch (crntScene)
	{
	case Scene::Battle:
		bkground.Load(L"images\\background.png");
		gameData.stage = Stage::Dark;

		player = new Player(Type::Fire, Type::Water);
		player->Init();
		enemies = new EnemyController();
		effects = new EffectManager();
		boss = new Boss();
		gui = new GUIManager(rectWindow);

		rectDisplay = gui->GetRectDisplay();

		SetTimer(hWnd, TIMERID_BATTLE_INVALIDATE, ELAPSE_BATTLE_INVALIDATE, T_Battle_Invalidate);
		SetTimer(hWnd, TIMERID_BATTLE_ANIMATION, ELAPSE_BATTLE_ANIMATION, T_Battle_Animate);
		SetTimer(hWnd, TIMERID_BATTLE_EFFECT, ELAPSE_BATTLE_EFFECT, T_Battle_Effect);
		SetTimer(hWnd, TIMERID_BATTLE_GUI, ELAPSE_BATTLE_GUI, T_Battle_GUI);
		SetTimer(hWnd, TIMERID_BATTLE_ANIMATION_BOSS, ELAPSE_BATTLE_ANIMATION_BOSS, T_Battle_AnimateBoss);

		soundManager->PlayBGMSound(BGMSound::Battle, 1.0f, true);
		soundManager->PlayEffectSound(EffectSound::Shot, 0.5f, true);
		break;
	}
}

void SceneManager::Init(const HWND& hWnd)
{
	soundManager = new SoundManager();

	GetClientRect(hWnd, &rectWindow);
	rectDisplay = rectWindow;
}

void SceneManager::Paint(const HWND& hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc, memDC;
	HBITMAP hBitmap;

	StartPaint(hWnd, ps, hdc, memDC, hBitmap, rectWindow);

	switch (crntScene)
	{
	case Scene::Battle:
		bkground.Draw(memDC, rectWindow);
		boss->Paint(memDC);
		player->Paint(memDC);
		enemies->Paint(memDC);
		player->PaintSkill(memDC);
		effects->Paint(memDC);
		gui->Paint(memDC);
		break;
	}

	FinishPaint(hWnd, ps, hdc, memDC, hBitmap, rectWindow);
}

void SceneManager::MoveScene(const HWND& hWnd, Scene scene)
{
	DeleteScene(hWnd);
	this->crntScene = scene;
	LoadScene(hWnd);
}