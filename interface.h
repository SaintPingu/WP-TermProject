#pragma once
class Player;

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000 ? 1 : 0))
#define KEYUP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000 ? 0 : 1))

#define KEY_UP VK_UP
#define KEY_LEFT VK_LEFT
#define KEY_DOWN VK_DOWN
#define KEY_RIGHT VK_RIGHT

typedef struct tagGameData {
	bool isGameStart = false;
	Difficulty difficulty = Difficulty::Easy;
	Scene scene = Scene::Stage;
	Stage stage = Stage::Empty;
}GameData;

void GameStart(HWND hWnd, GameData& data, Player& player);

void CheckKeyDown(HWND hWnd, const WPARAM& wParam, GameData& gameData);
void CheckKeyUp(HWND hWnd, const WPARAM& wParam, GameData& gameData);