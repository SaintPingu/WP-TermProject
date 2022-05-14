#pragma once
#include <Windows.h>
#include <tchar.h>
#include "flypokemon.h"

#define WINDOWSIZE_X 900
#define WINDOWSIZE_Y 1200

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000 ? 1 : 0))
#define KEYUP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000 ? 0 : 1))

#define KEY_UP 'W'
#define KEY_LEFT 'A'
#define KEY_DOWN 'S'
#define KEY_RIGHT 'D'

enum class Difficulty { Easy = 0, Normal, Hard };

typedef struct tagGameData {
	bool isGameStart = false;
	Difficulty difficulty = Difficulty::Easy;
}GameData;

void GameStart(HWND hWnd, GameData* data, FlyPokemon* flyPokemon);

void CheckKeyDown(HWND hWnd, const WPARAM& wParam, GameData* gameData, FlyPokemon* flyPokemon);
void CheckKeyUp(HWND hWnd, const WPARAM& wParam, GameData* gameData, FlyPokemon* flyPokemon);