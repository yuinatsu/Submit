#include <DxLib.h>
#include "Bullet.h"
#include "../Mng/ImageMng.h"
#include "../Mng/SceneMng.h"

Bullet::Bullet(const Vector2Flt& pos, const Vector2& size, const Object_ID oID, const Vector2Flt& speed, bool reverseXFlag,TeamTag tag):
	Object(pos,size,oID,0,tag), speed_(speed)
{
	reverseXFlag_ = reverseXFlag;
	lpImageMng.GetID("Bullet", "Resource/Image/player/cirno.png", Vector2{ 32,32 }, Vector2{ 1,1 });
	imageKey_ = "Bullet";
	animMax_ = 1;
	animFlam_ = 5;
}

Bullet::~Bullet()
{
}

void Bullet::Update(const double& delta, std::weak_ptr<MapData> mapData)
{
	pos_ += speed_;
	Vector2 checkPos = static_cast<Vector2>(pos_);
	const auto& ScrSize = lpSceneMng.GetScreenSize();
	if (checkPos.x<-(size_.x / 2) || checkPos.x>ScrSize.x + (size_.x / 2) || checkPos.y<-(size_.y / 2) || checkPos.y>ScrSize.y + (size_.y / 2))
	{
		isAlive_ = false;
	}
}

void Bullet::HitCollision(std::shared_ptr<Object> otehr)
{
	if (otehr->GetTeamTag() != teamTag_)
	{
		otehr->AddDamage(200);
		isAlive_ = false;
	}
}
