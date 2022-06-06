#include "stdafx.h"
#include "interface.h"
#include "player.h"
#include "image.h"
#include "timer.h"
#include "boss.h"

extern GameData gameData;
extern Player* player;
extern Boss* boss;

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
		gameData.isShowHitbox = !gameData.isShowHitbox;
		break;
	case _T('Q'):
		player->ActiveSkill(Skill::Identity);
		break;
	case _T('W'):
		player->ActiveSkill(Skill::Sector);
		break;
	case _T('E'):
		player->ActiveSkill(Skill::Circle);
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
	//constexpr int fieldLength = 720; // 1m/s
	constexpr int fieldLength = 1;

	constexpr int main_guiHeight = 80;

	constexpr int box_guiWidth = 92;
	constexpr int gague_guiWidth = 80;

	mainGUI = new GUIImage();
	gagueGUI_main = new GUIImage();
	gagueGUI_border = new GUIImage();
	gagueGUI_hp = new GUIImage();
	gagueGUI_mp = new GUIImage();
	
	icon_Q = new GUIImage();
	icon_W = new GUIImage();
	icon_E = new GUIImage();

	moveBarGUI = new GUIImage();
	icon_pokemon = new GUIImage();
	gaugeMoveBarGUI = new GUIImage();

	hurtGUI_Fire.gui = new GUIImage();
	hurtGUI_Water.gui = new GUIImage();
	hurtGUI_Elec.gui = new GUIImage();

	switch (player->GetType())
	{
	case Type::Elec:
		icon_Q->Load(_T("images\\icon_elec_Q.png"), { 35, 35 });
		icon_pokemon->Load(_T("images\\icon_thunder.png"), { 480, 480 });
		gaugeMoveBarGUI->Load(_T("images\\gague_bar_elec.png"), { 14, 217 });
		break;
	case Type::Water:
		icon_Q->Load(_T("images\\icon_water_Q.png"), { 260, 260 });
		icon_pokemon->Load(_T("images\\icon_articuno.png"), { 480, 480 });
		gaugeMoveBarGUI->Load(_T("images\\gague_bar_water.png"), { 14, 217 });
		break;
	case Type::Fire:
		icon_Q->Load(_T("images\\icon_fire_Q.png"), { 130, 130 });
		icon_pokemon->Load(_T("images\\icon_moltres.png"), { 480, 480 });
		gaugeMoveBarGUI->Load(_T("images\\gague_bar_fire.png"), { 14, 217 });
		break;
	}
	switch (player->GetSubType())
	{
	case Type::Elec:
		icon_W->Load(_T("images\\icon_elec_W.png"), { 130, 130 });
		icon_E->Load(_T("images\\icon_elec_E.png"), { 130, 130 });
		break;
	case Type::Water:
		icon_W->Load(_T("images\\icon_water_W.png"), { 130, 130 });
		icon_E->Load(_T("images\\icon_water_E.png"), { 130, 130 });
		break;
	case Type::Fire:
		icon_W->Load(_T("images\\icon_fire_W.png"), { 130, 130 });
		icon_E->Load(_T("images\\icon_fire_E.png"), { 130, 130 });
		break;
	}

	this->rectWindow = &rectWindow;
	rectMain = rectWindow;
	rectMain.top = rectMain.bottom - main_guiHeight;

	constexpr POINT skillBoxSize = { 38,39 };
	constexpr POINT boxStartPoint = { 59, 21 };
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

	constexpr POINT gagueSize = { 60,60 };
	constexpr POINT gagueStartPoint = { 30, 10 };
	rectHP.left = rectSkill_E.right + gagueStartPoint.x;
	rectHP.right = rectHP.left + gagueSize.x;
	rectHP.top = rectMain.top + gagueStartPoint.y;
	rectHP.bottom = rectHP.top + gagueSize.y;

	rectMP = rectHP;
	rectMP.left += gague_guiWidth;
	rectMP.right += gague_guiWidth;

	constexpr POINT moveBarSize = { 20, 220 };
	constexpr POINT moveBarStartPoint = { WINDOWSIZE_X - 40, 220 };
	rectMoveBar.left = moveBarStartPoint.x;
	rectMoveBar.top = moveBarStartPoint.y;
	rectMoveBar.right = rectMoveBar.left + moveBarSize.x;
	rectMoveBar.bottom = rectMoveBar.top + moveBarSize.y;

	constexpr POINT gagueMoveBarSize = { 14,214 };
	constexpr POINT gagueMoveBarStartPoint = { 3, 4 };
	rectGaugeMoveBar.left = rectMoveBar.left + gagueMoveBarStartPoint.x;
	rectGaugeMoveBar.top = rectMoveBar.top + gagueMoveBarStartPoint.y;
	rectGaugeMoveBar.right = rectGaugeMoveBar.left + gagueMoveBarSize.x;
	rectGaugeMoveBar.bottom = rectGaugeMoveBar.top + gagueMoveBarSize.y;

	constexpr POINT pokemonIconSize = { 24, 24 };
	const int pokemonIconCenterX = rectMoveBar.left + ((rectMoveBar.right - rectMoveBar.left) / 2);
	rectPokemonIcon.left = pokemonIconCenterX - (pokemonIconSize.x / 2);
	rectPokemonIcon.right = rectPokemonIcon.left + pokemonIconSize.x;
	rectPokemonIcon.top = rectMoveBar.bottom - (pokemonIconSize.y / 2);
	rectPokemonIcon.bottom = rectPokemonIcon.top + pokemonIconSize.y;

	constexpr float moveBarHeight = moveBarSize.y - moveBarSize.x;
	constexpr int corrValue = 5;
	iconMoveMaxY = (rectMoveBar.top - (pokemonIconSize.y / 2)) + corrValue;
	iconMoveMinY = (rectMoveBar.bottom + (pokemonIconSize.y / 2));
	iconMoveAmount = (moveBarHeight / fieldLength) / ELAPSE_GUI;

	mainGUI->Load(_T("images\\battleGUI.png"), { 500, 80 });
	gagueGUI_main->Load(_T("images\\gauge.png"), { 130, 130 });
	gagueGUI_border->Load(_T("images\\gauge_border.png"), { 130, 130 });
	gagueGUI_hp->Load(_T("images\\gauge_hp.png"), { 130, 130 });
	gagueGUI_mp->Load(_T("images\\gauge_mp.png"), { 130, 130 });
	moveBarGUI->Load(_T("images\\moveBar.png"), { 20, 223 });

	hurtGUI_Fire.gui->Load(_T("images\\frame_hurt_fire.png"), { 317, 564 }, 0);
	hurtGUI_Water.gui->Load(_T("images\\frame_hurt_water.png"), { 239, 371 }, 0);
	hurtGUI_Elec.gui->Load(_T("images\\frame_hurt_elec.png"), { 239, 371 }, 0);
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

	icon_Q->Paint(hdc, rectSkill_Q);
	icon_W->Paint(hdc, rectSkill_W);
	icon_E->Paint(hdc, rectSkill_E);

	if (boss->IsCreated() == false)
	{
		moveBarGUI->Paint(hdc, rectMoveBar);
		gaugeMoveBarGUI->PaintGauge(hdc, rectGaugeMoveBar, iconMoveMinY - rectPokemonIcon.bottom, iconMoveMaxY);
		icon_pokemon->Paint(hdc, rectPokemonIcon);
	}

	hurtGUI_Fire.gui->Paint(hdc, *rectWindow);
	hurtGUI_Water.gui->Paint(hdc, *rectWindow);
	hurtGUI_Elec.gui->Paint(hdc, *rectWindow);
}

void GUIManager::Update()
{
	hurtGUI_Fire.ReduceAlpha();
	hurtGUI_Water.ReduceAlpha();
	hurtGUI_Elec.ReduceAlpha();

	if (isIconStop == true)
	{
		return;
	}
	rectPokemonIcon.top -= iconMoveAmount;
	rectPokemonIcon.bottom -= iconMoveAmount;
	if (rectPokemonIcon.top < iconMoveMaxY)
	{
		const int corrValue = rectPokemonIcon.top - iconMoveMaxY;
		rectPokemonIcon.top += corrValue;
		rectPokemonIcon.bottom += corrValue;
		isIconStop = true;

		BossData bossData;
		bossData.hp = 5000;
		bossData.damage = 2;
		bossData.damage_skill1 = 5;
		bossData.damage_skill2 = 2;
		bossData.speed = 1;
		bossData.bulletSpeed[static_cast<int>(BossAct::Line)] = 6;
		bossData.bulletSpeed[static_cast<int>(BossAct::Sector)] = 3;
		bossData.bulletSpeed[static_cast<int>(BossAct::Circle)] = 4;
		bossData.bulletSpeed[static_cast<int>(BossAct::Spiral)] = 5;
		bossData.bulletSpeed[static_cast<int>(BossAct::Spread)] = 6;

		bossData.actDelay = 3000;
		//bossData.crntActDelay = bossData.actDelay;
		bossData.crntActDelay = 0; // debug
		bossData.attackDelay[static_cast<int>(BossAct::Line)] = 40;
		bossData.attackDelay[static_cast<int>(BossAct::Sector)] = 250;
		bossData.attackDelay[static_cast<int>(BossAct::Circle)] = 200;
		bossData.attackDelay[static_cast<int>(BossAct::Spiral)] = 10;
		bossData.attackDelay[static_cast<int>(BossAct::Spread)] = 10;

		bossData.frameNum_Idle = 0;
		switch (gameData.stage)
		{
		case Stage::Elec:
			bossData.type = Type::Elec;
			bossData.frameNum_IdleMax = 2;
			bossData.frameNum_Atk = 3;
			bossData.frameNum_AtkMax = 5;
			bossData.frameNum_AtkRev = 5;
			break;
		case Stage::Water:
			bossData.type = Type::Water;
			bossData.frameNum_IdleMax = 2;
			bossData.frameNum_Atk = 3;
			bossData.frameNum_AtkMax = 3;
			bossData.frameNum_AtkRev = 3;
			break;
		case Stage::Fire:
			bossData.type = Type::Fire;
			bossData.frameNum_IdleMax = 1;
			bossData.frameNum_Atk = 2;
			bossData.frameNum_AtkMax = 6;
			bossData.frameNum_AtkRev = 6;
			break;
		}
	
		boss->Create(bossData);
	}
}
RECT GUIManager::GetRectDisplay() const
{
	RECT rectDisplay = *rectWindow;
	rectDisplay.bottom = rectMain.top;
	return rectDisplay;	
}

void GUIManager::DisplayHurtFrame(Type type)
{
	switch (type)
	{
	case Type::Fire:
		hurtGUI_Fire.alpha = hurtGUI_alpha;
		break;
	case Type::Water:
		hurtGUI_Water.alpha = hurtGUI_alpha;
		break;
	case Type::Elec:
		hurtGUI_Elec.alpha = hurtGUI_alpha;
		break;
	default:
		assert(0);
		break;
	}
}




void GUIManager::HurtGUI::ReduceAlpha()
{
	if (alpha == 0x00)
	{
		return;
	}
	gui->SetAlpha(alpha--);
}