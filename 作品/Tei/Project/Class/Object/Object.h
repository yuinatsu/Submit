#pragma once
#include <memory>
#include <vector>
#include <list>
#include <map>
#include <functional>
#include <string>
#include "../common/Vector2.h"
#include "../Input/INPUT_ID.h"
#include "TeamTag.h"
#include "../Animation/Anim_ID.h"
#include "characterID.h"

class MapData;

enum class DIR
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	MAX,
};

class Collision;
class SquaerCollision;


class Object
{
public:
	Object(const Vector2Flt& pos,const Vector2& size,const Object_ID oID,int hp,TeamTag tag);
	virtual ~Object() = default;
	virtual void Update(const double& delta,std::weak_ptr<MapData> mapData) = 0;
	virtual void Draw(const double& delta);
	/// <summary>
	/// ダメージ受ける奴はoverrideして処理を書きましょう
	/// </summary>
	virtual void AddDamage(int damage);
	/// <summary>
	/// Collisionが当たった時に呼び出される関数 必要なクラスはoverrideして処理を書きましょう
	/// </summary>
	virtual void HitCollision(std::shared_ptr<Object> otehr);
	
	Vector2 GetSize(void);

	TeamTag GetTeamTag(void);

	bool Alive(void);
	
private:
protected:

	int hp_;

	Vector2Flt pos_;
	Vector2 size_;

	double delta_;
	double invincibleCnt_;

	Object_ID objectID_;
	TeamTag teamTag_;

	std::string imageKey_;


	bool reverseXFlag_;
	bool isAlive_;

	double exRate_;
	double angle_;

	int animCnt_;
	int animMax_;
	int animFlam_;

	friend class SquaerCollision;
};

