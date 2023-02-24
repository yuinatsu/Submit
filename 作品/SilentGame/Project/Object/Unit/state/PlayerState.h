#pragma once
#include <Dxlib.h>
#include <map>
#include <vector>
#include <string>
#include <functional>
#include "../Player.h"
#include "../../UI/UiBase.h"
#include "../../../Input/InputID.h"
#include "../../../Scene/SceneMng.h"
#include "../../../common/AnimMng.h"
#include "../../../Component/Animation.h"
#include "../../../common/nameof.hpp"
#include "../../../common/rapidxml.hpp"
#include "../../../common/rapidxml_utils.hpp"
#include "../../../common/Debug.h"
#include "../../../Component/Collider/CircleCollider.h"

// 移動
struct Move
{
	bool operator()(Obj* obj, rapidxml::xml_node<>* node)
	{
		for (auto atr = node->first_attribute(); atr != nullptr; atr = atr->next_attribute())
		{
			std::string name = atr->name();
			std::string turn = atr->value();
			obj->speed_ = (float)std::atoi(atr->value());
			// X座標
			if (name == "x")
			{
				// 移動量計算
				auto tmp = obj->speed_ * obj->spMag_ * obj->delta_;
				obj->pos_.x += tmp;
				obj->pos_.Normalized();
				float val{ obj->spMag_ };
				if (obj->spMag_ <= 0.4f)
				{
					val = 0.0f;
				}
				if (obj->IncludeComponent(ComponentID::Collider))
				{
					dynamic_cast<CircleCollider&>(obj->GetComponent(ComponentID::Collider, 2)).SetRadius(60.0f * val);
				}
				return true;
			}
			// Y座標
			else if (name == "y")
			{
				// 移動量計算
				auto tmp = obj->speed_ * obj->spMag_ * obj->delta_;
				obj->pos_.y += tmp;
				obj->pos_.Normalized();
				float val{ obj->spMag_ };
				if (obj->spMag_ <= 0.4f)
				{
					val = 0.0f;
				}
				if (obj->IncludeComponent(ComponentID::Collider))
				{
					dynamic_cast<CircleCollider&>(obj->GetComponent(ComponentID::Collider, 2)).SetRadius(60.0f * val);
				}
				return true;
			}
		}
		if (obj->IncludeComponent(ComponentID::Collider))
		{
			dynamic_cast<CircleCollider&>(obj->GetComponent(ComponentID::Collider, 2)).SetRadius(0.0f);
		}
		return false;
	}
};


// 移動量変化
struct SpChange
{
	bool operator()(Obj* obj, rapidxml::xml_node<>* node)
	{
		for (auto atr = node->first_attribute(); atr != nullptr; atr = atr->next_attribute())
		{
			std::string name = atr->name();
			std::string state = atr->value();
			// エフェクト名検索
			if (name == "effect")
			{
				for (auto tmp : EffectName::Max)
				{
					if (state == NAMEOF_ENUM(tmp))
					{
						obj->effect_ = tmp;
					}
				}
			}
			// 移動スピード更新
			if (name == "speed")
			{
				obj->spMag_ = static_cast<float>(std::atof(atr->value()));
				return true;
			}
		}
		return false;
	}
};


// キーチェック
struct CheckKey
{
	bool operator()(Obj* obj, rapidxml::xml_node<>* node)
	{
		for (auto atr = node->first_attribute(); atr != nullptr; atr = atr->next_attribute())
		{
			std::string name = atr->value();
			auto player = dynamic_cast<Player*>(obj);
			// キー検索
			// 速度変化重複回避
			if (name == "Dash" && player->controller_->Press(InputID::Crouch))
			{
				return false;
			}
			if (name == "Crouch" && player->controller_->Press(InputID::Dash))
			{
				return false;
			}
			for (auto tmp : InputID::Max)
			{
				if (name == NAMEOF_ENUM(tmp))
				{
					return player->controller_->Press(tmp);
				}
			}
			// その他input情報
			if (name == "Walk")
			{
				// 通常移動（Shift、Ctrlがニュートラル）
				return player->controller_->WalkPress();
			}
			else if (name == "Non")
			{
				// すべてのキーがニュートラル
				return player->controller_->IsNeutral();
			}
			else if (name == "Move")
			{
				// up,down,left,rightキーが押されてる
				return player->controller_->MovePress(true);
			}
			else if (name == "NotMove")
			{
				// up,down,left,rightキーがニュートラル
				return player->controller_->MovePress(false);
			}
		}
		return false;
	}
};


// アニメーションのセット
struct SetAnime
{
	bool operator()(Obj* obj, rapidxml::xml_node<>* node)
	{
		for (auto atr = node->first_attribute(); atr != nullptr; atr = atr->next_attribute())
		{
			std::string name = atr->name();
			std::string tmp = atr->value();
			// ステータス更新
			if (name == "state")
			{
				// Idle時はアニメーションしないようにする
				if (tmp == "Idle")
				{
					dynamic_cast<Unit*>(obj)->SetIdleFlag(true);
					obj->effect_ = EffectName::Non;
					if (obj->IncludeComponent(ComponentID::Collider))
					{
						dynamic_cast<CircleCollider&>(obj->GetComponent(ComponentID::Collider, 2)).SetRadius(0.0f);
					}
				}
				// ステータスセット
				else 
				{
					dynamic_cast<Animation&>(obj->GetComponent(ComponentID::Animator)).SetState(tmp);
					dynamic_cast<Unit*>(obj)->SetIdleFlag(false);
				}
				return true;
			}
		}
		return false;
	}
};


// 当たり判定チェック
struct ColisionCheck
{
	bool operator()(Obj* obj, rapidxml::xml_node<>* node)
	{
		Math::Vector2 vec(0, 0);
		// ステージ全体サイズ
		Math::Vector2I windowSize = Math::Vector2{1920,1088};
		auto window = [&](Math::Vector2 v)
		{
			return (v >= Math::Vector2{0,0} && v <= Math::Vector2(windowSize));
		};
		Math::Vector2 size;
		float speed = 0;
		auto checkMove = [&](Math::Vector2 moveVec)
		{
			MapCollMng::Ray ray = { { obj->pos_ }, moveVec };
			auto unit = dynamic_cast<Unit*>(obj);

			// ステージコリジョン当たり判定
			for (Collision col : unit->tmx_->GetColList())
			{
				if (MapCollMng::CheckCollision(ray, col).first)
				{
					return false;
				}
			}
			
			return true;
		};

		for (auto atr = node->first_attribute(); atr != nullptr; atr = atr->next_attribute())
		{
			std::string name = atr->name();
			std::string tmp = atr->value();
			// 値セット
			if (name == "speed")
			{
				speed = static_cast<float>(atoi(tmp.c_str())) * obj->delta_;
			}
			if (name == "width")
			{
				vec.x = static_cast<float>(atoi(tmp.c_str()));
				vec.x += speed * obj->delta_;
			}
			if (name == "height")
			{
				vec.y = static_cast<float>(atoi(tmp.c_str()));
				vec.y += speed * obj->delta_;
			}
			// 攻撃判定
			if (name == "attack")
			{
				for (int i = 0; i < 4; i++)
				{
					if (i == 0)
					{
						vec.y = static_cast<float>(atoi(tmp.c_str()));
						vec.y *= -1;
						vec.y -= speed;
					}
					else if (i == 1)
					{
						vec.y = static_cast<float>(atoi(tmp.c_str()));
						vec.y += speed;
					}
					else if (i == 2)
					{
						vec.x = static_cast<float>(atoi(tmp.c_str()));
						vec.x *= -1;
						vec.x -= speed;
					}
					else if (i == 3)
					{
						vec.x = static_cast<float>(atoi(tmp.c_str()));
						vec.x += speed;
					}
					if (!(checkMove(vec) && window(obj->pos_ + vec)))
					{
						return true;
					}
					vec = { 0,0 };
				}
				return false;
			}
		}
		if (checkMove(vec) && window(obj->pos_ + vec))
		{
			return true;
		}
		return false;
	}
};


struct SetMoveVec
{
	bool operator()(Obj* obj, rapidxml::xml_node<>* node)
	{
		auto x{ std::stof(node->first_attribute("dirX")->value()) };
		auto y{ std::stof(node->first_attribute("dirY")->value()) };
		obj->SetMoveVec(obj->GetMoveVec() + Math::Vector2{ x,y });
		return true;
	}
};

struct MovePos
{
	bool operator()(Obj* obj, rapidxml::xml_node<>* node)
	{
		if (obj->GetMoveVec().SqMagnitude() == 0.0f)
		{
			dynamic_cast<Unit*>(obj)->SetIdleFlag(true);
			obj->effect_ = EffectName::Non;
			if (obj->IncludeComponent(ComponentID::Collider))
			{
				dynamic_cast<CircleCollider&>(obj->GetComponent(ComponentID::Collider, 2)).SetRadius(0.0f);
			}
			return false;
		}
		dynamic_cast<Unit*>(obj)->SetIdleFlag(false);
		obj->moveVec_.Normalize();
		MapCollMng::Ray ray{ obj->GetPos()  ,obj->GetMoveVec() * (obj->delta_ * obj->GetSpeed() * obj->GetSpMag() + 7.5f) };
		for (auto& c : dynamic_cast<Unit*>(obj)->tmx_->GetColList())
		{
			const auto& [flag, moveV, colV] = MapCollMng::CheckColl2(ray, c);
			if (flag)
			{
				//obj->pos_ += moveV;
				// dynamic_cast<Player*>(obj)->dbgTmpPos_ = obj->GetPos() + ray.vec;
				
				/*DebugLog("当たり", obj->moveVec_.x, ":", obj->moveVec_.y);*/
				return true;
			}
		}

		obj->Obj::MovePos(obj->delta_);
		return true;
	}
};

struct CorrectAnim
{
	bool operator()(Obj* obj, rapidxml::xml_node<>* node)
	{
		auto ckFunc = [this, obj](const Math::Vector2& rV, const Math::Vector2& lV)
		{
			if (Math::Cross(obj->GetMoveVec(), rV) < 0.0f && Math::Cross(obj->GetMoveVec(), lV) > 0.0f)
			{
				return true;
			}
			return false;
		};

		if (ckFunc(Math::downVector2<float> +Math::rightVector2<float>, Math::downVector2<float> +Math::leftVector2<float>))
		{
			dynamic_cast<Animation&>(obj->GetComponent(ComponentID::Animator)).SetState("Down");
			return true;
		}

		if (ckFunc(Math::upVector2<float> +Math::leftVector2<float>, Math::upVector2<float> +Math::rightVector2<float>))
		{
			dynamic_cast<Animation&>(obj->GetComponent(ComponentID::Animator)).SetState("Up");
			return true;
		}

		if (ckFunc(Math::downVector2<float> +Math::leftVector2<float>, Math::upVector2<float> +Math::leftVector2<float>))
		{

			dynamic_cast<Animation&>(obj->GetComponent(ComponentID::Animator)).SetState("Left");
			return true;
		}

		if (ckFunc(Math::upVector2<float> +Math::rightVector2<float>, Math::downVector2<float> +Math::rightVector2<float>))
		{
			dynamic_cast<Animation&>(obj->GetComponent(ComponentID::Animator)).SetState("Right");
			return true;
		}
		return false;
	}
};

struct Attack
{
	bool operator()(Obj* obj, rapidxml::xml_node<>* node)
	{
		if (obj->IsAttack())
		{
			auto player = dynamic_cast<Player*>(obj);
			player->attackStepTime_ += obj->delta_;
			auto timeMax{ std::stof(node->first_attribute("time")->value()) };
			if (timeMax <= player->attackStepTime_)
			{
				player->SetAttackFlag(false);
				player->attackStepTime_ = 0.0f;
			}
		}
		return true;
	}
};

struct animNode
{
	bool operator()(Obj* obj, rapidxml::xml_node<>* node)
	{
		for (auto attach = node->first_node(); attach != nullptr; attach = attach->next_sibling())
		{
			// 名前をとる
			std::string attachName = attach->first_attribute("name")->value();
			// 動きが登録されていたら
			if (animNode_.find(attachName) != animNode_.end())
			{
				// 正しく動作しなかったら次に回る
				if (!animNode_[attachName](obj, attach))
				{
					continue;
				}
				if (!(*this)(obj, attach))
				{
					return false;
				}
			}
		}
		return true;
	}
	std::map<std::string, std::function<bool(Obj* obj, rapidxml::xml_node<>* node)>> animNode_ = {
		{"Move",Move()},
		{"SpChange",SpChange()},
		{"CheckKey",CheckKey()},
		{"SetAnime",SetAnime()},
		{"ColisionCheck",ColisionCheck()},
		{"SetMoveVec", SetMoveVec()},
		{"MovePos",MovePos()},
		{"CorrectAnim", CorrectAnim()},
		{"Attack", Attack()}
	};
};
