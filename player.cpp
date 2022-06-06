#include "stdafx.h"
#include "player.h"
#include "bullet.h"
#include "timer.h"
#include "skill.h"
#include "effect.h"
#include "interface.h"

extern GUIManager* gui;
extern EffectManager* effects;

Player::Player( PlayerData data)
{
	this->data = data;

	direction = Dir::Empty;
	posDest = { 0, };
	vectorMove = { 0, };
	alpha = 0;
	StopMove();
}
void Player::Init(const RECT& rectDisplay)
{
	constexpr int damagePer_ms = (1000 / ELAPSE_ANIMATION);
	this->rectDisplay = &rectDisplay;

	static ObjectImage img_pokemon;
	ObjectImage bulletImage;
	ObjectImage subBulletImage;

	switch (data.type)
	{
	case Type::Elec:
		pokemon = Pokemon::Thunder;
		img_pokemon.Load(_T("images\\sprite_thunder.png"), { 53, 48 }, { 19, 10 }, { 17,24 });
		bulletImage.Load(_T("images\\bullet_elec_main.png"), { 5,16 });

		data.maxhp = 40;
		data.maxmp = 80;
		data.mp = 0;
		data.speed = 4;
		data.damage = 1.25f;
		data.damage_Q = 10.5f / damagePer_ms;
		data.bulletSpeed = 8;
		data.shotDelay = 90;
		break;
	case Type::Water:
		pokemon = Pokemon::Articuno;
		img_pokemon.Load(_T("images\\sprite_articuno.png"), { 69, 69 }, { 29, 28 }, { 13,23 });
		img_pokemon.ScaleImage(1.2f, 1.2f);
		bulletImage.Load(_T("images\\bullet_ice.png"), { 7,15 });
		bulletImage.ScaleImage(0.9f, 0.9f);

		data.maxhp = 70;
		data.maxmp = 120;
		data.mp = 40;
		data.speed = 2.5f;
		data.damage = 1;
		data.damage_Q = 3.85f / damagePer_ms;
		data.bulletSpeed = 6;
		data.shotDelay = 110;
		break;
	case Type::Fire:
		pokemon = Pokemon::Moltres;
		img_pokemon.Load(_T("images\\sprite_moltres.png"), { 83, 75 }, { 35, 25 }, { 15,28 });
		bulletImage.Load(_T("images\\bullet_fire.png"), { 11,16 });
		bulletImage.ScaleImage(0.9f, 0.9f);

		data.maxhp = 55;
		data.maxmp = 100;
		data.mp = 20;
		data.speed = 3;
		data.damage = 1.25f;
		data.damage_Q = 15.5f / damagePer_ms;
		data.bulletSpeed = 7;
		data.shotDelay = 100;
		break;
	default:
		assert(0);
		break;
	}
	//data.maxhp = 1000000; // debug
	//data.mp = 100; // debug
	data.hp = data.maxhp;

	switch (data.subType)
	{
	case Type::Elec:
		subPokemon = SubPokemon::Pikachu;
		img_subPokemon.Load(L"images\\sub_pikachu.png", { 23,25 });
		subBulletImage.Load(_T("images\\bullet_elec.png"), { 11,30 });
		subBulletImage.ScaleImage(0.7f, 0.7f);
		break;
	case Type::Water:
		subPokemon = SubPokemon::Squirtle;
		img_subPokemon.Load(L"images\\sub_squirtle.png", { 17,24 });
		subBulletImage.Load(_T("images\\bullet_water.png"), { 8,24 });
		subBulletImage.ScaleImage(0.8f, 0.7f);
		break;
	case Type::Fire:
		subPokemon = SubPokemon::Charmander;
		img_subPokemon.Load(L"images\\sub_charmander.png", { 18,23 });
		subBulletImage.Load(_T("images\\bullet_flame.png"), { 8,16 });
		subBulletImage.ScaleImage(0.7f, 0.7f);
		break;
	default:
		assert(0);
		break;
	}
	data.subDamage = 1;
	data.damage_WE = 1.1f;

	GameObject::Init(img_pokemon, { 250,500 });
	bullets = new PlayerBullet(rectDisplay, bulletImage);
	subBullets = new PlayerBullet(rectDisplay, subBulletImage);
	skillManager = new SkillManager();
}

void Player::Paint(HDC hdc)
{
	RECT rectImage =ISprite::GetRectImage(GetImage(), frame);
	GameObject::Paint(hdc, &rectImage);

	const ObjectImage& image = GetImage();
	float scaleX, scaleY;
	image.GetScale(scaleX, scaleY);

	RECT rectDest = GetRectBody();
	switch (pokemon)
	{
	case Pokemon::Moltres:
		rectDest.top += 7 * scaleY;
		rectDest.bottom = rectDest.top + (22 * scaleY);
		break;
	case Pokemon::Articuno:
		rectDest.right += 2;
		rectDest.top += 3 * scaleY;
		rectDest.bottom = rectDest.top + (22 * scaleY);
		break;
	case Pokemon::Thunder:
		rectDest.top += 5 * scaleY;
		rectDest.bottom = rectDest.top + (20 * scaleY);
		break;
	default:
		assert(0);
		break;
	}
	img_subPokemon.Paint(rectDest, hdc);

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

	posDest = Vector2::GetDest(GetPosCenter(), vectorMove);
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
	else if (rectBody.right > rectDisplay->right)
	{
		corrValue.x = rectDisplay->right - rectBody.right;
	}
	if (rectBody.top < 0)
	{
		corrValue.y = -rectBody.top;
	}
	else if (rectBody.bottom > rectDisplay->bottom)
	{
		corrValue.y = rectDisplay->bottom - rectBody.bottom;
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
inline bool Player::IsClearShotDelay() const
{
	return (data.crntShotDelay <= 0);
}
inline void Player::ResetShotDelay()
{
	data.crntShotDelay = data.shotDelay;
}
void Player::Shot()
{
	RECT rectBody = GetRectBody();
	BulletData bulletData;
	bulletData.bulletType = data.type;
	bulletData.damage = data.damage;
	bulletData.speed = data.bulletSpeed;

	POINT bulletPos = { 0, };
	bulletPos.y = rectBody.top;
	bulletPos.x = rectBody.left - 10;
	bullets->CreateBullet(bulletPos, bulletData, Dir::Up);
	bulletPos.x = rectBody.right + 10;
	bullets->CreateBullet(bulletPos, bulletData, Dir::Up);

	bulletData.bulletType = data.subType;
	bulletData.damage = data.subDamage;
	bulletPos.x = rectBody.left + ((rectBody.right - rectBody.left) / 2);
	subBullets->CreateBullet(bulletPos, bulletData, Dir::Up);

	skillManager->UseSkill();
}
void Player::CheckShot()
{
	data.crntShotDelay -= ELAPSE_INVALIDATE;
	if (IsClearShotDelay() == true)
	{
		Shot();
		ResetShotDelay();
	}
}
void Player::CreateSubBullet(POINT center, const BulletData& data, Vector2 unitVector, bool isRotateImg, bool isSkillBullet)
{
	subBullets->CreateBullet(center, data, unitVector, isRotateImg, isSkillBullet);
}

void Player::MoveBullets()
{
	bullets->Move();
	subBullets->Move();
}

void Player::Hit(float damage, Type hitType, POINT effectPoint)
{
	if (effectPoint.x == -1)
	{
		effectPoint = GetPosCenter();
		GetRandEffectPoint(effectPoint);
	}
	effects->CreateHitEffect(effectPoint, hitType);
	gui->DisplayHurtFrame(hitType);

	damage = CalculateDamage(damage, data.type, hitType);
	if ((data.hp -= damage) <= 0)
	{
		data.speed = 0;
	}
}

void Player::ActiveSkill(Skill skill)
{
	skillManager->ActiveSkill(skill);
}

bool Player::IsUsingSkill() const
{
	return skillManager->IsUsingSkill();
}