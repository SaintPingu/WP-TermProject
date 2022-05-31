#include "stdafx.h"
#include "interface.h"
#include "player.h"
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
	case _T('W'):
		player->UseSkill(Skill::Sector);
		break;
	case _T('E'):
		player->UseSkill(Skill::Circle);
		break;
	}

	if (KEYDOWN(KEY_LEFT))
	{
		player->Stop(Dir::Right);
		player->SetDirection(Dir::Left);
	}
	if (KEYDOWN(KEY_RIGHT))
	{
		if (wParam != KEY_LEFT)
		{
			player->Stop(Dir::Left);
			player->SetDirection(Dir::Right);
		}
	}
	if (KEYDOWN(KEY_UP))
	{
		player->Stop(Dir::Down);
		player->SetDirection(Dir::Up);
	}
	if (KEYDOWN(KEY_DOWN))
	{
		if (wParam != KEY_UP)
		{
			player->Stop(Dir::Up);
			player->SetDirection(Dir::Down);
		}
	}
	player->SetMove(hWnd, TIMERID_MOVE_PLAYER, ELAPSE_MOVE_PLAYER, T_MovePlayer);
	InvalidateRect(hWnd, NULL, FALSE);
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