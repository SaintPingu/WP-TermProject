#include "interface.h"

void GameStart(HWND hWnd, GameData* data, FlyPokemon* flyPokemon)
{
	data->isGameStart = true;

}
void CheckKeyDown(HWND hWnd, const WPARAM& wParam, GameData* gameData, FlyPokemon* flyPokemon)
{
	if (gameData->isGameStart == false)
	{
		switch (wParam)
		{
		case _T('s'):
		case _T('S'):
			gameData->isGameStart = true;
			break;
		}

		InvalidateRect(hWnd, NULL, FALSE);
		return;
	}

	switch (wParam)
	{
	case _T('H'):
		flyPokemon->ShowHitbox();
		break;
	}

	if (KEYDOWN(KEY_LEFT))
	{
		flyPokemon->Stop(hWnd, Dir::Right);
		flyPokemon->SetDirection(Dir::Left);
	}
	if (KEYDOWN(KEY_RIGHT))
	{
		if (wParam != KEY_LEFT)
		{
			flyPokemon->Stop(hWnd, Dir::Left);
			flyPokemon->SetDirection(Dir::Right);
		}
	}
	if (KEYDOWN(KEY_UP))
	{
		flyPokemon->Stop(hWnd, Dir::Down);
		flyPokemon->SetDirection(Dir::Up);
	}
	if (KEYDOWN(KEY_DOWN))
	{
		if (wParam != KEY_UP)
		{
			flyPokemon->Stop(hWnd, Dir::Up);
			flyPokemon->SetDirection(Dir::Down);
		}
	}
	flyPokemon->SetMove(hWnd, TIMERID_MOVE_PLAYER, ELAPSE_MOVE_PLAYER, T_MovePlayer);
	InvalidateRect(hWnd, NULL, FALSE);
}
void CheckKeyUp(HWND hWnd, const WPARAM& wParam, GameData* gameData, FlyPokemon* flyPokemon)
{
	if (gameData->isGameStart == false)
	{
		return;
	}
	else if (flyPokemon->IsMove() == true)
	{
		switch (wParam)
		{
		case KEY_LEFT:
			flyPokemon->Stop(hWnd, Dir::Left);
			if (KEYDOWN(KEY_RIGHT))
			{
				flyPokemon->SetDirection(Dir::Right);
			}
			break;
		case KEY_RIGHT:
			flyPokemon->Stop(hWnd, Dir::Right);
			if (KEYDOWN(KEY_LEFT))
			{
				flyPokemon->SetDirection(Dir::Left);
			}
			break;
		case KEY_UP:
			flyPokemon->Stop(hWnd, Dir::Up);
			if (KEYDOWN(KEY_DOWN))
			{
				flyPokemon->SetDirection(Dir::Down);
			}
			break;
		case KEY_DOWN:
			flyPokemon->Stop(hWnd, Dir::Down);
			if (KEYDOWN(KEY_UP))
			{
				flyPokemon->SetDirection(Dir::Up);
			}
			break;
		}
	}
}