#include <DxLib.h>
#include <math.h>
#include "../../common/GmkEvent.h"
#include "SecurityCam.h"
#include "../../Component/Collider/FanCollider.h"
#include "../../common/SoundPross.h"


Alert::Alert(std::weak_ptr< GmkEvent>& gmkEvent, Math::Vector2 pos, Math::Vector2 vec,std::shared_ptr<TmxObj>& tmxObj, std::weak_ptr<Obj>&& player) :
	 tmxObj_(tmxObj), player_(player), GimicObj{gmkEvent,pos}
{
	stopFlag_ = false;
	time_ = 0;
	sFlag_ = false;
	// fan_ = std::make_unique<Fan>(pos, vec * Math::Vector2{112,112}, DX_PI_F / 5.0f, tmxObj_->GetColList(), tmxObj_->GetCol2List());
	auto v1 = vec * 112.0f;
	auto v2 = (Math::RotateMat(DX_PI_F / 5.0f) * v1);
	
	AddComponent(std::make_unique<FanCollider>(tmxObj_, v1, v2, CheckType::SecurityCam, std::bind(&Alert::Hit, this, std::placeholders::_1), *this));
	Init();

}


Alert::~Alert()
{
}

void Alert::Init(void)
{
	sig_ = 1.0f;
}

void Alert::Update(float delta)
{
	// 視線にプレイヤーがいなければうろうろする
	if (!sFlag_)
	{
		UpdateNoDis(player_.lock()->GetPos(), delta);
	}
	else
	{
		UpdateDis(player_.lock()->GetPos(), delta);
	}
}


// 見つけていないときの処理
void Alert::UpdateNoDis(const Math::Vector2& pos, float delta)
{
	/*float Deg = Math::GetNormalizedAngle(fan_->GetAngle(), Math::Deg2Rad(0.0f), Math::Deg2Rad(360.0f));
	float Rad = Math::Rad2Deg(Deg);
	auto flag = fan_->UpdateHitRect(pos);*/
	time_ += delta;

	/*if (flag && rotFlag_ && rotTime_ <= -1.0f)
	{
		rotTime_ = 1.0f;
		rotFlag_ = false;
	}
	else if (flag && !rotFlag_ && rotTime_ <= -1.0f)
	{
		rotTime_ = 1.0f;
		rotFlag_ = true;
	}
	if (rotFlag_ && rotTime_ <= 0.0f)
	{
		fan_->AddAngle(0.003f);

	}
	if (!rotFlag_ && rotTime_ <= 0.0f)
	{
		fan_->AddAngle(-0.003f);

	}*/

	if (!stopFlag_)
	{
		// 左右のベクトルをチェックし障害物に触れていたら反対方向に進
		auto& fan = dynamic_cast<FanCollider&>(GetComponent(ComponentID::Collider));
		if (fan.CheckCol())
		{
			sig_ = sig_ * -1.0f;
			stopFlag_ = true;
			time_ = 0.0f;
		}
		fan.AddAngle(sig_ * 0.003f);
	}
	else
	{
		if (time_ > 3.0f)
		{
			stopFlag_ = false;
		}
	}

	/*if (fan_->UpdateHitPos(pos))
	{
		sFlag_ = true;
		if (!gmkEvent_.expired())
		{
			gmkEvent_.lock()->StartEvent(pos, EventType::Alert);
		}
	}*/
}

// 見つけた時の処理
void Alert::UpdateDis(const Math::Vector2& pos, float delta)
{
	if (sFlag_ && time_ <= 4)
	{
		
		if (time_ == 0.0f)
		{
			pflag_ = true;
		}
		else
		{
			pflag_ = false;
		}
		time_ += delta;
	}
	else
	{
		time_ = 0.0f;
		sFlag_ = false;
	}
}

VECTOR V2V(const Math::Vector2& v)
{
	return VGet(v.x, v.y, 0);
}

void Alert::Draw(const Math::Vector2& offset, DrawMng& drawMng)
{
	if (!sFlag_)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
		// fan_->Draw(offset);
		auto& fan = dynamic_cast<FanCollider&>(GetComponent(ComponentID::Collider));
		//GetComponent(ComponentID::Collider).DbgDraw(offset);
		auto color = 0xff0000;
		constexpr float min_angle = DX_PI_F / 36.0f;	// だいたい５度
		float angle = Math::GetAngle2Vector(fan.GetV1Vec(), fan.GetV2Vec());

		int triangles_num = static_cast<int>(ceil(angle / min_angle));
		std::vector<VERTEX2D> v(3 * triangles_num);

		for (auto& e : v)
		{
			e.rhw = 1.0f;
			e.dif = GetColorU8(color >> 16 & 0xff, color >> 8 & 0xff, color & 0xff, 255);
			e.u = 0;
			e.v = 0;
		}
		auto vstart = fan.GetV1Vec();
		auto vend = Math::RotateMat(min_angle) * fan.GetV1Vec();
		for (size_t i = 0; i < triangles_num; i++)
		{
			if (i < triangles_num - 1)
			{
				v[i * 3 + 0].pos = V2V(pos_ + offset);
				v[i * 3 + 1].pos = V2V(pos_ + offset + vstart);
				v[i * 3 + 2].pos = V2V(pos_ + offset + vend);
			}
			else
			{
				v[i * 3 + 0].pos = V2V(pos_ + offset);
				v[i * 3 + 1].pos = V2V(pos_ + offset + vstart);
				v[i * 3 + 2].pos = V2V(pos_ + offset + fan.GetV2Vec());
			}
			vstart = vend;
			vend = Math::RotateMat(min_angle) * vstart;
		}
		// 2Dのポリゴンを描画
		DrawPrimitive2D(v.data(), static_cast<int>(v.size()), DX_PRIMTYPE_TRIANGLELIST, DX_NONE_GRAPH, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
}


bool Alert::GetpFlag(void)
{
	return pflag_;
}

void Alert::Hit(Collider& other)
{
	if (sFlag_)
	{
		return;
	}

	sFlag_ = true;
	if (!gmkEvent_.expired())
	{
		pflag_ = true;
		time_ = 0.0f;
		gmkEvent_.lock()->StartEvent(other.GetOwner().GetPos(), EventType::Alert);
	}
}

