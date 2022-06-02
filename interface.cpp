#include "stdafx.h"
#include "interface.h"
#include "player.h"
#include "image.h"
#include "timer.h"

extern Player* player;

void GameStart(HWND hWnd, GameData& data, Player& player)
{
	data.isGameStart = true;

}
void CheckKeyDown(HWND hWnd, const WPARAM& wParam, GameData& gameData)
{
	if (gameData.isGameStart == false)
	{
		switch (wParam)
		{
		case _T('s'):
		case _T('S'):
			gameData.isGameStart = true;
			break;
		}

		InvalidateRect(hWnd, NULL, FALSE);
		return;
	}

	switch (wParam)
	{
	case _T('H'):
		player->ShowHitbox();
		break;
	case _T('Q'):
		player->UseSkill(Skill::Identity);
		break;
	case _T('W'):
		player->UseSkill(Skill::Sector);
		break;
	case _T('E'):
		player->UseSkill(Skill::Circle);
		break;
	}

	bool isMove = false;
	if (KEYDOWN(KEY_LEFT))
	{
		player->Stop(Dir::Right);
		player->SetDirection(Dir::Left);
		isMove = true;
	}
	if (KEYDOWN(KEY_RIGHT))
	{
		if (wParam != KEY_LEFT)
		{
			player->Stop(Dir::Left);
			player->SetDirection(Dir::Right);
			isMove = true;
		}
	}
	if (KEYDOWN(KEY_UP))
	{
		player->Stop(Dir::Down);
		player->SetDirection(Dir::Up);
		isMove = true;
	}
	if (KEYDOWN(KEY_DOWN))
	{
		if (wParam != KEY_UP)
		{
			player->Stop(Dir::Up);
			player->SetDirection(Dir::Down);
			isMove = true;
		}
	}
	if (isMove == true)
	{
		player->SetMove(hWnd, TIMERID_MOVE_PLAYER, ELAPSE_MOVE_PLAYER, T_MovePlayer);
	}
}
void CheckKeyUp(HWND hWnd, const WPARAM& wParam, GameData& gameData)
{
	if (gameData.isGameStart == false)
	{
		return;
	}
	else if (player->IsMove() == true)
	{
		switch (wParam)
		{
		case KEY_LEFT:
			player->Stop(Dir::Left);
			if (KEYDOWN(KEY_RIGHT))
			{
				player->SetDirection(Dir::Right);
			}
			break;
		case KEY_RIGHT:
			player->Stop(Dir::Right);
			if (KEYDOWN(KEY_LEFT))
			{
				player->SetDirection(Dir::Left);
			}
			break;
		case KEY_UP:
			player->Stop(Dir::Up);
			if (KEYDOWN(KEY_DOWN))
			{
				player->SetDirection(Dir::Down);
			}
			break;
		case KEY_DOWN:
			player->Stop(Dir::Down);
			if (KEYDOWN(KEY_UP))
			{
				player->SetDirection(Dir::Up);
			}
			break;
		}
	}
}

GUIManager::GUIManager(const RECT& rectWindow)
{
	constexpr int main_guiHeight = 97;

	constexpr int box_guiWidth = 79;
	constexpr int gague_guiWidth = 70;

	mainGUI = new GUIImage();
	gagueGUI_main = new GUIImage();
	gagueGUI_border = new GUIImage();
	gagueGUI_hp = new GUIImage();
	gagueGUI_mp = new GUIImage();

	this->rectWindow = &rectWindow;
	rectMain = rectWindow;
	rectMain.top = rectMain.bottom - main_guiHeight;

	constexpr POINT skillBoxSize = { 53,55 };
	constexpr POINT boxStartPoint = { 30, 21 };
	rectSkill_Q.left = rectMain.left + boxStartPoint.x;
	rectSkill_Q.top = rectMain.top + boxStartPoint.y;
	rectSkill_Q.right = rectSkill_Q.left + skillBoxSize.x;
	rectSkill_Q.bottom = rectSkill_Q.top + skillBoxSize.y;

	rectSkill_W = rectSkill_Q;
	rectSkill_W.left += box_guiWidth;
	rectSkill_W.right += box_guiWidth + 1;

	rectSkill_E = rectSkill_W;
	rectSkill_E.left += box_guiWidth;
	rectSkill_E.right += box_guiWidth;

	rectSkill_R = rectSkill_E;
	rectSkill_R.left += box_guiWidth;
	rectSkill_R.right += box_guiWidth;

	constexpr POINT gagueSize = { 65,65 };
	constexpr POINT gagueStartPoint = { 12, 15 };
	rectHP.left = rectSkill_R.right + gagueStartPoint.x;
	rectHP.right = rectHP.left + gagueSize.x;
	rectHP.top = rectMain.top + gagueStartPoint.y;
	rectHP.bottom = rectHP.top + gagueSize.y;

	rectMP = rectHP;
	rectMP.left += gague_guiWidth;
	rectMP.right += gague_guiWidth;

	mainGUI->Load(_T("images\\battleGUI.png"), { 500, 97 });
	gagueGUI_main->Load(_T("images\\gauge.png"), { 130, 130 });
	gagueGUI_border->Load(_T("images\\gauge_border.png"), { 130, 130 });
	gagueGUI_hp->Load(_T("images\\gauge_hp.png"), { 130, 130 });
	gagueGUI_mp->Load(_T("images\\gauge_mp.png"), { 130, 130 });
}
void GUIManager::Paint(HDC hdc)
{
	mainGUI->Paint(hdc, rectMain);

	gagueGUI_main->Paint(hdc, rectHP);
	gagueGUI_hp->PaintGauge(hdc, rectHP, player->GetHP(), player->GetMaxHP());
	gagueGUI_border->Paint(hdc, rectHP);

	gagueGUI_main->Paint(hdc, rectMP);
	gagueGUI_mp->PaintGauge(hdc, rectMP, player->GetMP(), player->GetMaxMP());
	gagueGUI_border->Paint(hdc, rectMP);
}
RECT GUIManager::GetRectDisplay() const
{
	RECT rectDisplay = *rectWindow;
	rectDisplay.bottom = rectMain.top;
	return rectDisplay;	
}