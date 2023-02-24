#include "../Component/Collider/CircleCollider.h"
#include "../Component/Animation.h"
#include "../common/ImageMng.h"
#include "../common/DrawMng.h"
#include "../Scene/SceneMng.h"
#include "Unit/Enemy.h"
#include "../Resource/Tmx/TmxObj.h"
#include "Decoration.h"





Decoration::Decoration(const std::string& fname, const std::string& state, const std::string& key, const Math::Vector2& pos) :
	Obj{pos}
{
	InitAnim(fname, state, key);
}

Decoration::Decoration(const std::string& fname, const std::string& state, const std::string& key, std::shared_ptr<TmxObj>& tmxObj, float r, const Math::Vector2& pos) :
	Obj{pos}
{
	InitCollision(tmxObj, r);
	InitAnim(fname, state, key);
}

Decoration::Decoration(const std::string& fname, const std::string& key, const Math::Vector2I& divSize, const Math::Vector2I& divNum, int idx, const Math::Vector2& pos) :
	Obj{pos}
{
	lpImageMng.GetID(fname, key, divSize, divNum);
	drawFunc_ = [key, idx, this](const Math::Vector2& offset, DrawMng& drawMng)
	{
		drawMng.Draw(pos_ + offset, 1.0f,moveVec_.GetAngle(), lpImageMng.GetID(key)[idx]);
	};
}


Decoration::Decoration(const std::string& fname, const std::string& key, const Math::Vector2I& divSize, const Math::Vector2I& divNum, int idx, std::shared_ptr<TmxObj>& tmxObj, float r, const Math::Vector2& pos) :
	Obj{pos}
{
	InitImage(fname, key, divSize, divNum, idx);
	InitCollision(tmxObj,r);
}

Decoration::~Decoration()
{
}

bool Decoration::InitAnim(const std::string& fname, const std::string& state, const std::string& key)
{
	bool rtn{ AddComponent(std::make_unique<Animation>(fname,key,state,*this)) };

	drawFunc_ = [this](const Math::Vector2& offset, DrawMng& drawMng)
	{
		dynamic_cast<Animation&>(GetComponent(ComponentID::Animator)).Draw(drawMng, pos_ + offset, false);
		//GetComponent(ComponentID::Collider).DbgDraw(offset);
	};
	dynamic_cast<Animation&>(GetComponent(ComponentID::Animator)).Update(lpSceneMng.GetRandom().GetRandom(0.0f,60.0f));
	return rtn;
}

bool Decoration::InitCollision(std::shared_ptr<TmxObj>& tmxObj, float r)
{
	bool rtn{ AddComponent(std::make_unique<CircleCollider>(
		r,
		CheckType::Box,
		[this,tmxObj](Collider& other) {

			if (other.GetMyCheckType() == CheckType::EnemyBody)
			{
				if (dynamic_cast<Enemy&>(other.GetOwner()).GetEnemyState() == EnemyState::Gimic)
				{
					return;
				}
			}
			auto tmp = other.GetOwner().GetPos() - pos_;
			tmp.Normalize();
			auto length = dynamic_cast<CircleCollider&>(other).GetRadius() +
				dynamic_cast<CircleCollider&>(GetComponent(ComponentID::Collider)).GetRadius();
			
			other.GetOwner().SetPos(pos_ + (tmp * length));

		},
		*this)) };

	dynamic_cast<Collider&>(GetComponent(ComponentID::Collider)).SetTargetType(static_cast<int>(CheckType::EnemyBody) | static_cast<int>(CheckType::PlayerBody));

	return rtn;
}

bool Decoration::InitImage(const std::string& fname, const std::string& key, const Math::Vector2I& divSize, const Math::Vector2I& divNum, int idx)
{
	lpImageMng.GetID(fname, key, divSize, divNum);
	drawFunc_ = [key,idx,this](const Math::Vector2& offset, DrawMng& drawMng)
	{
		drawMng.Draw(pos_ + offset, moveVec_.GetAngle(), lpImageMng.GetID(key)[idx]);
	};
	return true;
}



void Decoration::Draw(const Math::Vector2& offset, DrawMng& drawMng)
{
	drawFunc_(offset, drawMng);
}
