#pragma once
class Player;

#define WINDOWSIZE_X 900
#define WINDOWSIZE_Y 1200

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000 ? 1 : 0))
#define KEYUP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000 ? 0 : 1))

#define KEY_UP 'W'
#define KEY_LEFT 'A'
#define KEY_DOWN 'S'
#define KEY_RIGHT 'D'

typedef struct tagGameData {
	bool isGameStart = false;
	Difficulty difficulty = Difficulty::Easy;
	Scene scene = Scene::Stage;
	Stage stage = Stage::Empty;
}GameData;

void GameStart(HWND hWnd, GameData& data, Player& player);

void CheckKeyDown(HWND hWnd, const WPARAM& wParam, GameData& gameData, Player& player);
void CheckKeyUp(HWND hWnd, const WPARAM& wParam, GameData& gameData, Player& player);