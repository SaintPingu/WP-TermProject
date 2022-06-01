#include "stdafx.h"
#include "player.h"
#include "bullet.h"
#include "timer.h"
#include "skill.h"

Player::Player(HWND hWnd, const RECT& rectWindow, ObjectImage& image, float scaleX, float scaleY, Vector2 pos, PlayerData data) : GameObject(image, scaleX, scaleY, pos)
{
	this->rectWindow = &rectWindow;
	this->data = data;

	direction = Dir::Empty;
	posDest = { 0, };
	vectorMove = { 0, };
	alpha = 0;
	StopMove();

	subPokemon = SubPokemon::Squirtle;
	img_subPokemon[static_cast<int>(SubPokemon::Pikachu)].Load(L"images\\sub_pikachu.png", { 23,25 });
	img_subPokemon[static_cast<int>(SubPokemon::Squirtle)].Load(L"images\\sub_squirtle.png", { 17,24 });
	img_subPokemon[static_cast<int>(SubPokemon::Charmander)].Load(L"images\\sub_charmander.png", { 18,23 });

	pokemon = Pokemon::Moltres;
	ObjectImage bulletImage;

	switch (pokemon)
	{
	case Pokemon::Moltres:
		type = Type::Fire;
		bulletImage.Load(_T("images\\bullet_fire.png"), { 11,16 });
		bulletImage.ScaleImage(0.9f, 0.9f);
		break;
	case Pokemon::Articuno:
		type = Type::Water;
		bulletImage.Load(_T("images\\bullet_ice.png"), { 7,15 });
		bulletImage.ScaleImage(0.9f, 0.9f);
		break;
	case Pokemon::Thunder:
		type = Type::Elec;
		bulletImage.Load(_T("images\\bullet_elec_main.png"), { 5,16 });
		break;
	default:
		assert(0);
		break;
	}
	bullets = new PlayerBullet(rectWindow, bulletImage);

	ObjectImage subBulletImage;
	switch (subPokemon)
	{
	case SubPokemon::Pikachu:
		subBulletImage.Load(_T("images\\bullet_elec.png"), { 11,30 });
		subBulletImage.ScaleImage(0.7f, 0.7f);
		subBullets = new PlayerBullet(rectWindow, subBulletImage);
		subType = Type::Elec;
		break;
	case SubPokemon::Charmander:
		subBulletImage.Load(_T("images\\bullet_flame.png"), { 8,16 });
		subBulletImage.ScaleImage(0.7f, 0.7f);
		subBullets = new PlayerBullet(rectWindow, subBulletImage);
		subType = Type::Fire;
		break;
	case SubPokemon::Squirtle:
		subBulletImage.Load(_T("images\\bullet_water.png"), { 8,24 });
		subBulletImage.ScaleImage(0.8f, 0.7f);
		subBullets = new PlayerBullet(rectWindow, subBulletImage);
		subType = Type::Water;
		break;
	default:
		assert(0);
		break;
	}

	skillManager = new SkillManager(this, subType);
}

void Player::Paint(HDC hdc)
{
	RECT rectImage = GetRectImage(GetImage(), frame);
	GameObject::Paint(hdc, &rectImage);

	const ObjectImage& image = GetImage();
	float scaleX, scaleY;
	image.GetScale(scaleX, scaleY);

	RECT rectDest = GetRectBody();
	switch (pokemon)
	{
	case Pokemon::Moltres:
		rectDest.left += 2;
		rectDest.right -= 2;
		rectDest.top += 8 * scaleY;
		rectDest.bottom = rectDest.top + (18 * scaleY);
		break;
	case Pokemon::Articuno:
		rectDest.right += 2;
		rectDest.top += 3 * scaleY;
		rectDest.bottom = rectDest.top + (22 * scaleY);
		break;
	case Pokemon::Thunder:
		rectDest.left += 2;
		rectDest.right -= 2;
		rectDest.top += 5 * scaleY;
		rectDest.bottom = rectDest.top + (20 * scaleY);
		break;
	default:
		assert(0);
		break;
	}
	img_subPokemon[static_cast<int>(subPokemon)].Paint(rectDest, hdc);

	bullets->Paint(hdc);
	subBullets->Paint(hdc);
}
void Player::PaintSkill(HDC hdc)
{
	skillManager->Paint(hdc);
}

void Player::SetPosDest()
{
	const int amount = data.speed * 2;
	switch (direction)
	{
	case Dir::Left:
		vectorMove.x = -amount;
		break;
	case Dir::Right:
		vectorMove.x = amount;
		break;
	case Dir::Up:
		vectorMove.y = -amount;
		break;
	case Dir::Down:
		vectorMove.y = amount;
		break;
	case Dir::LD:
		vectorMove.x = -amount;
		vectorMove.y = amount;
		break;
	case Dir::LU:
		vectorMove.x = -amount;
		vectorMove.y = -amount;
		break;
	case Dir::RD:
		vectorMove.x = amount;
		vectorMove.y = amount;
		break;
	case Dir::RU:
		vectorMove.x = amount;
		vectorMove.y = -amount;
		break;
	default:
		break;
	}

	Vector2 posCenter = GetPosCenter();
	posDest = posCenter + vectorMove;
}

void Player::SetDirection(Dir inputDir)
{
	if (direction == inputDir || direction == Dir::Empty)
	{
		direction = inputDir;
		return;
	}

	switch (direction)
	{
	case Dir::Left:
		if (inputDir == Dir::Up)		direction = Dir::LU;
		else if (inputDir == Dir::Down)	direction = Dir::LD;
		break;
	case Dir::Right:
		if (inputDir == Dir::Up)		direction = Dir::RU;
		else if (inputDir == Dir::Down)	direction = Dir::RD;
		break;
	case Dir::Up:
		if (inputDir == Dir::Left)		direction = Dir::LU;
		else if (inputDir == Dir::Right)direction = Dir::RU;
		break;
	case Dir::Down:
		if (inputDir == Dir::Left)		direction = Dir::LD;
		else if (inputDir == Dir::Right)direction = Dir::RD;
		break;
	default:
		return;
	}
	StopMove();
}

void Player::SetMove(HWND hWnd, int timerID, int elpase, TIMERPROC timerProc)
{
	if (IsMove() == true && alpha > 0)
	{
		return;
	}

	SetPosDest();

	if (IsMove() == false && alpha == 0)
	{
		SetTimer(hWnd, timerID, elpase, timerProc);
	}
	else if (alpha > 0.5f)
	{
		alpha = 0.5f;
	}
	StartMove();
}

void Player::Move(HWND hWnd, int timerID)
{
	Vector2 posCenter = GetPosCenter();
	Vector2 posNext = Lerp(posCenter, posDest, alpha);

	posNext = CheckCollideWindow(posNext);
	SetPos(posNext);
	posCenter = GetPosCenter();
	vectorMove = posDest - posCenter;

	alpha += 0.1f;
	if (alpha > 0.5f)
	{
		if (direction != Dir::Empty)
		{
			SetPosDest();

			alpha = 0.5f;
		}
		else if (alpha > 1)
		{
			vectorMove = { 0, };
			StopMove();
			alpha = 0;
			KillTimer(hWnd, timerID);
		}
	}
}

void Player::Stop(Dir inputDir)
{
	switch (direction)
	{
	case Dir::Left:
		if (inputDir != Dir::Left) return;
		break;
	case Dir::Right:
		if (inputDir != Dir::Right) return;
		break;
	case Dir::Up:
		if (inputDir != Dir::Up) return;
		break;
	case Dir::Down:
		if (inputDir != Dir::Down) return;
		break;
	case Dir::LU:
		if (inputDir != Dir::Left && inputDir != Dir::Up) return;
		break;
	case Dir::LD:
		if (inputDir != Dir::Left && inputDir != Dir::Down) return;
		break;
	case Dir::RU:
		if (inputDir != Dir::Right && inputDir != Dir::Up) return;
		break;
	case Dir::RD:
		if (inputDir != Dir::Right && inputDir != Dir::Down) return;
		break;
	default:
		break;
	}
	direction = direction - inputDir;
}

Vector2 Player::CheckCollideWindow(Vector2 pos) const
{
	RECT rectBody = GetRectBody(pos);

	POINT corrValue = { 0, };
	if (rectBody.left < 0)
	{
		corrValue.x = -rectBody.left;
	}
	else if (rectBody.right > rectWindow->right)
	{
		corrValue.x = rectWindow->right - rectBody.right;
	}
	if (rectBody.top < 0)
	{
		corrValue.y = -rectBody.top;
	}
	else if (rectBody.bottom > rectWindow->bottom)
	{
		corrValue.y = rectWindow->bottom - rectBody.bottom;
	}

	return { pos.x + corrValue.x, pos.y + corrValue.y };
}

void Player::Animate()
{
	if (isRevFrame == true)
	{
		--frame;
	}
	else
	{
		++frame;
	}

	switch (GetAction())
	{
	case Action::Idle:
		if (frame > 2)
		{
			isRevFrame = true;
			--frame;
		}
		else if (frame < 0)
		{
			isRevFrame = false;
			++frame;
		}
		break;
	default:
		assert(0);
		break;
	}

	skillManager->Animate();
}
void Player::Fire()
{
	RECT rectBody = GetRectBody();
	BulletData bulletData;
	bulletData.bulletType = type;
	bulletData.damage = data.damage;
	bulletData.speed = 7;

	POINT bulletPos = { 0, };
	bulletPos.y = rectBody.top;
	bulletPos.x = rectBody.left - 10;
	bullets->CreateBullet(bulletPos, bulletData, Dir::Up);
	bulletPos.x = rectBody.right + 10;
	bullets->CreateBullet(bulletPos, bulletData, Dir::Up);

	bulletData.bulletType = subType;
	bulletPos.x = rectBody.left + ((rectBody.right - rectBody.left) / 2);
	subBullets->CreateBullet(bulletPos, bulletData, Dir::Up);

	switch (crntSkill)
	{
	case Skill::Sector:
		FireBySector();
		break;
	case Skill::Circle:
		FireByCircle();
		break;
	default:
		break;
	}
}

void Player::MoveBullets()
{
	bullets->Move();
	subBullets->Move();
}

void Player::Hit(float damage, Type hitType)
{
	CalculateDamage(damage, type, hitType);
	if ((data.hp -= damage) <= 0)
	{
		data.speed = 0;
	}
}

void Player::FireBySector()
{
	RECT rectBody = GetRectBody();
	BulletData bulletData;
	bulletData.bulletType = subType;
	bulletData.damage = data.damage;
	bulletData.speed = 10;

	POINT bulletPos = { 0, };
	bulletPos.y = rectBody.top;
	bulletPos.y = rectBody.top;
	bulletPos.x = rectBody.left + ((rectBody.right - rectBody.left) / 2);	

	Vector2 unitVector = Vector2::Forward();
	int startDegree = -10 - (skillCount * 10);
	int rotationDegree = -(startDegree * 2) / 10;
	unitVector = Rotate(unitVector, startDegree);
	for (int i = 0; i < 11; ++i)
	{
		subBullets->CreateBullet(bulletPos, bulletData, unitVector, true);
		unitVector = Rotate(unitVector, rotationDegree);
	}

	if (--skillCount <= 0)
	{
		skillCount = 0;
		crntSkill = Skill::Empty;
	}
}
void Player::FireByCircle()
{
	POINT bulletPos = GetPosCenter();
	BulletData bulletData;
	bulletData.bulletType = subType;
	bulletData.damage = data.damage;
	bulletData.speed = 10;

	Vector2 unitVector = Vector2::Forward();
	unitVector = Rotate(unitVector, skillCount * 6);
	for (int i = 0; i < 18; ++i)
	{
		subBullets->CreateBullet(bulletPos, bulletData, unitVector, true);
		unitVector = Rotate(unitVector, 20);
	}

	if (--skillCount <= 0)
	{
		skillCount = 0;
		crntSkill = Skill::Empty;
	}
}

void Player::UseSkill(Skill skill)
{
	if (crntSkill != Skill::Empty)
	{
		return;
	}

	crntSkill = skill;
	switch (skill)
	{
	case Skill::Sector:
		skillCount = 5;
		break;
	case Skill::Circle:
		skillCount = 5;
		break;
	case Skill::Identity:
		skillManager->UseSkill(Skill::Identity);
		crntSkill = Skill::Empty;
		break;
	default:
		assert(0);
		break;
	}
}