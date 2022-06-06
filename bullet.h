#pragma once
#include "image.h"

typedef struct BulletData {
	Type bulletType = Type::Empty;
	float damage = 0;
	float speed = 0;
}BulletData;

class BulletController abstract : public ISprite {
protected:
	class Bullet {
	private:
		BulletData data;
		bool isSkillBullet = false;
		bool isRotateImg = false;

		Dir dir = Dir::Empty;
		Vector2 unitVector;

		FRECT rectBody = { 0, };
		FRECT rectRotBody = { 0, };
		Bullet(POINT center, POINT bulletSize, const BulletData& data);
	public:
		Bullet(POINT center, POINT bulletSize, const BulletData& data, Dir dir) : Bullet(center, bulletSize, data) { this->dir = dir; };
		Bullet(POINT center, POINT bulletSize, const BulletData& data, Vector2 unitVector, bool isRotateImg, bool isSkillBullet = false);
		~Bullet() {};

		void Paint(HDC hdc, const ObjectImage& bulletImage, const RECT& rectWindow);
		bool Move(const RECT& rectWindow);
		bool IsCollide(const RECT& rect) const;

		inline float GetDamage() const
		{
			return data.damage;
		}
		inline Dir GetDir() const
		{
			return dir;
		}
		inline RECT GetRect() const
		{
			return rectRotBody;
		}
		inline Type GetType() const
		{
			return data.bulletType;
		}
		inline bool IsSkillBullet() const
		{
			return isSkillBullet;
		}
		POINT GetPos() const;
	};

	BulletController(const RECT& rectDisplay, const ObjectImage& bulletImage);

	std::vector<Bullet*> bullets;
	ObjectImage bulletImage;
	POINT bulletSize = { 0, };

	const RECT* rectDisplay = nullptr;

	void Pop(size_t& index);
public:

	void Paint(HDC hdc);

	void CreateBullet(POINT center, const BulletData& data, Dir dir);
	void CreateBullet(POINT center, const BulletData& data, Vector2 unitVector, bool isRotateImg = false, bool isSkillBullet = false);
	void DestroyCollideBullet(const RECT& rect);

	virtual void Move() abstract;
};

class PlayerBullet : public BulletController {
public:
	PlayerBullet(const RECT& rectDisplay, const ObjectImage& bulletImage) : BulletController(rectDisplay, bulletImage) {};
	void Move() override;
};
class EnemyBullet : public BulletController {
public:
	EnemyBullet(const RECT& rectDisplay, const ObjectImage& bulletImage) : BulletController(rectDisplay, bulletImage) {};
	void Move() override;
	POINT GetBulletSize() const
	{
		return bulletImage.GetDrawSize();
	}
};