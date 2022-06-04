#pragma once
class Player;
class GUIImage;

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

class GUIManager {
private:
	const RECT* rectWindow = nullptr;
	float iconMoveMaxY = 0;
	float iconMoveMinY = 0;
	float iconMoveAmount = 0;
	bool isIconStop = false;

	RECT rectMain = { 0, };
	RECT rectSkill_Q = { 0, };
	RECT rectSkill_W = { 0, };
	RECT rectSkill_E = { 0, };
	RECT rectHP = { 0, };
	RECT rectMP = { 0, };
	RECT rectMoveBar = { 0, };
	RECT rectGaugeMoveBar = { 0, };
	FRECT rectPokemonIcon = { 0, };

	GUIImage* mainGUI = nullptr;
	GUIImage* gagueGUI_main = nullptr;
	GUIImage* gagueGUI_border = nullptr;
	GUIImage* gagueGUI_hp = nullptr;
	GUIImage* gagueGUI_mp = nullptr;

	GUIImage* icon_Q = nullptr;
	GUIImage* icon_W = nullptr;
	GUIImage* icon_E = nullptr;

	GUIImage* moveBarGUI = nullptr;
	GUIImage* gaugeMoveBarGUI = nullptr;
	GUIImage* icon_pokemon = nullptr;
public:
	GUIManager(const RECT& rectWindow);
	void Paint(HDC hdc);
	void Update();
	RECT GetRectDisplay() const;
};

void GameStart(HWND hWnd, GameData& data, Player& player);

void CheckKeyDown(HWND hWnd, const WPARAM& wParam, GameData& gameData);
void CheckKeyUp(HWND hWnd, const WPARAM& wParam, GameData& gameData);