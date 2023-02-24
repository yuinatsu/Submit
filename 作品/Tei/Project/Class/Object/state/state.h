#pragma once
#include <string>
#include <map>
#include <functional>
#include <cassert>
#include "../Pawn.h"
#include "../Bullet.h"
#include "../../collision/Collision.h"
#include "../../collision/SquaerCollision.h"
#include "../../Mng/CollisionMng.h"
#include "../../Map/MapData.h"
#include "../../common/Raycast.h"
#include "../../Input/Controller.h"
#include "../../XML/RapidXml/rapidxml.hpp"
#include "../../XML/RapidXml/rapidxml_utils.hpp"
#include "../../../_debug/_DebugConOut.h"
#include "../../../_debug/_DebugDispOut.h"


namespace
{
	std::map<std::string, INPUT_ID>  input_IDkey_ = {
		{"LEFT",INPUT_ID::LEFT},
		{"RIGHT",INPUT_ID::RIGHT},
		{"UP",INPUT_ID::UP},
		{"DOWN",INPUT_ID::DOWN},
		{"SWT1",INPUT_ID::SWT1},
		{"SWT2",INPUT_ID::SWT2},
		{"SWT3",INPUT_ID::SWT3}
	};
	std::map<std::string, Char_Anim_ID>  Anim_IDkey_ = {
		{"Idle",Char_Anim_ID::Idle},
		{"Run",Char_Anim_ID::Run},
		{"Jump",Char_Anim_ID::Jump},
		{"Fall",Char_Anim_ID::Fall}
	};
}
namespace state
{
	struct CheckRay
	{
		bool operator()(Pawn* pawn, rapidxml::xml_node<>* node)
		{
			Vector2Flt moveVec = { 0.0f,0.0f };
			Vector2Flt offset = { 0.0f,0.0f };
			for (auto atr = node->first_attribute(); atr != nullptr; atr = atr->next_attribute())
			{
				std::string name = atr->name();
				if (name == "x")
				{
					moveVec.x = static_cast<float>(std::atof(atr->value()))*pawn->delta_;
				}
				if (name == "y")
				{
					moveVec.y = static_cast<float>(std::atof(atr->value())) * pawn->delta_;
				}
				if (name == "offsetX")
				{
					std::string str = atr->value();
					if (str == "+")
					{
						moveVec.x += pawn->size_.x / 2.0f;
					}
					if (str == "-")
					{
						moveVec.x += -(pawn->size_.x / 2.0f);
					}
					offset = { 0.0f,pawn->size_.y / 2.0f };

				}
				if (name == "offsetY")
				{
					std::string str = atr->value();
					if (str == "+")
					{
						moveVec.y += pawn->size_.y / 2.0f;
					}
					if (str == "-")
					{
						moveVec.y += -(pawn->size_.y / 2.0f);
					}
					offset = { pawn->size_.x / 2.0f ,0.0f };
				}

			}
			Vector2Flt tmpPos = pawn->pos_ - offset;

			for (int i = 0; i < 9; i++)
			{
				Raycast::Ray ray = { tmpPos ,moveVec };
				//_dbgDrawLine(ray.point.x, ray.point.y, ray.point.x + ray.vec.x, ray.point.y + ray.vec.y, 0x00ff00);
				for (auto colData : pawn->mapData_.lock()->GetColData())
				{
					if (pawn->raycast_->CheckCollision(ray, colData))
					{
						//_dbgDrawBox(colData.first.x, colData.first.y, colData.first.x + colData.second.x, colData.first.y + colData.second.y, 0xffffff, true);
						return false;
					}
				}
				tmpPos += offset / 4.0f;
			}

			return true;
		}
	};

	struct CheckG
	{
		bool operator()(Pawn* pawn, rapidxml::xml_node<>* node)
		{
			for (auto colData : pawn->mapData_.lock()->GetColData())
			{
				//_dbgDrawBox(colData.first.x, colData.first.y, colData.first.x + colData.second.x, colData.first.y + colData.second.y, 0xff0000, false);
			}
			std::string atr = node->first_attribute("isGnd")->value();
			bool reFlag = false;
			if (atr=="true")
			{
				reFlag = true;
			}
			Vector2Flt moveVec = { 0.0f,(pawn->size_.y / 2.0f) + pawn->yaddPower_ };
			Vector2Flt offset = { pawn->size_.x / 2.0f ,0.0f };
			if (pawn->yaddPower_ <= 0)
			{
				moveVec = { 0.0f,-(pawn->size_.y / 2.0f) - pawn->yaddPower_ };
			}

			Vector2Flt tmpPos = pawn->pos_ - offset;

			for (int i = 0; i < 9; i++)
			{
				Raycast::Ray ray = { tmpPos ,moveVec };
				//_dbgDrawLine(ray.point.x, ray.point.y, ray.point.x + ray.vec.x, ray.point.y + ray.vec.y, 0x00ff00);
				for (auto colData : pawn->mapData_.lock()->GetColData())
				{
					//_dbgDrawBox(colData.first.x, colData.first.y, colData.first.x + colData.second.x, colData.first.y + colData.second.y, 0xff0000, false);
					if (pawn->raycast_->CheckCollision(ray, colData))
					{
						pawn->yaddPower_ = 0.2f;
						return reFlag;
					}
				}
				tmpPos += offset / 4.0f;
			}
			return !reFlag;
		}
	};

	struct Attack
	{
		bool operator()(Pawn* pawn, rapidxml::xml_node<>* node)
		{
			TRACE("ƒXƒyƒ‹ƒJ[ƒh”­“®\n");
			if (!pawn->bullet_)
			{
				Vector2Flt speed = {};
				if (pawn->reverseXFlag_)
				{
					speed = { -15.0f,0.0f };
				}
				else
				{
					speed = { 15.0f,0.0f };
				}
				pawn->bullet_ = std::make_shared<Bullet>(pawn->pos_, Vector2{ 32,32 }, Object_ID::Bullet, speed, pawn->reverseXFlag_, pawn->teamTag_);
				Vector2Flt size = static_cast<Vector2Flt>(pawn->bullet_->GetSize());
				auto col = std::make_shared<SquaerCollision>(size, size / 2.0f);
				col->SetOwner(pawn->bullet_);
				lpCollisionMng.RegistrationCol(col);
			}
			return true;
		}
	};

	struct Command
	{
		bool operator()(Pawn* pawn, rapidxml::xml_node<>* node)
		{
			auto CheckCommand = [&](CommandData data)
			{
				auto input = pawn->controller_->GetRingBuf();
				unsigned int timeCnt_ = 0;
				for (auto cmd = data.command_.rbegin(); cmd != data.command_.rend(); cmd++)
				{
					int cnt = 0;
					unsigned int checkID = static_cast<unsigned int>(cmd->id);
					unsigned int mask = 0xffffffff;
					// ƒ{ƒ^ƒ““ü—ÍŽž‚Í•ûŒü“ü—Í‚ð–³Ž‹‚·‚é
					if (input->id_ >= static_cast<unsigned int>(CMD_ID::SWT3))
					{
						mask -= 0x0000000f;
					}
					// ¶‰E”½“]ˆ—
					if (pawn->reverseXFlag_ && ((cmd->id >= CMD_ID::LEFT) && cmd->id <= CMD_ID::RIGHT_D_D))
					{
						checkID ^= 0x0000000c;
					}
					// ”»’è Ý’è‚³‚ê‚½Time•ª‚Ü‚Å‘k‚é
					while (((input->id_ & mask)) == checkID && (cnt < cmd->time))
					{
						input = input->befor_;
						cnt++;
					};
					// “ü—Í‚ª‚ ‚Á‚½‚©
					if (cnt == 0)
					{
						// ‚»‚ê‚Í•K{‚È‚Ì‚©
						if (cmd->required_)
						{
							//TRACE(("COMMANDŽ¸”sI" + data.name_ + ": % d\n").c_str(), checkID);
							return false;
						}
					}
					timeCnt_ += cnt;
				}
				if (timeCnt_ > data.allTime_)
				{
					//TRACE(("COMMANDŽ¸”sI" + data.name_ + ":‘S‘Ì“ü—ÍŽžŠÔ‚ª’·‚·‚¬:%d/%d\n").c_str(), timeCnt_, data.allTime_);
					return false;
				}
				//TRACE(("COMMAND¬Œ÷I" + data.name_ + "\n").c_str());
				TRACE((data.name_ + "\n").c_str());
				return true;
			};
			
			for (const auto& data:pawn->commandList_)
			{
				if (CheckCommand(data))
				{
					return true;
				}
			}
			return false;
		}
	};

	struct CheckAnim
	{
		bool operator()(Pawn* pawn, rapidxml::xml_node<>* node)
		{
			std::string flag = node->first_attribute("flag")->value();
			bool reFlag = false;
			if (flag == "true")
			{
				reFlag = true;
			}
			for (auto atr = node->first_attribute(); atr != nullptr; atr = atr->next_attribute())
			{
				std::string name = atr->name();
				if (name == "id")
				{
					std::string val = atr->value();
					if (pawn->animID_ == Anim_IDkey_[val])
					{
						return reFlag;
					}
				}
			}
			return !reFlag;
		}
	};

	struct CheckAlive
	{
		bool operator()(Pawn* pawn, rapidxml::xml_node<>* node)
		{
			std::string check = node->first_attribute("flag")->value();
			bool reflag = true;
			if (check == "false")
			{
				reflag = false;
			}
			if (pawn->isAlive_)
			{
				return reflag;
			}
			return !reflag;
		}
	};

	struct Move
	{
		bool operator()(Pawn* pawn, rapidxml::xml_node<>* node)
		{
			for (auto atr = node->first_attribute(); atr != nullptr; atr = atr->next_attribute())
			{
				std::string name = atr->name();
				if (name == "x")
				{
					float movePow = static_cast<float>(std::atof(atr->value()));
					pawn->pos_.x += movePow*pawn->delta_;
					if (movePow < 0)
					{
						pawn->reverseXFlag_ = true;
					}
					else
					{
						pawn->reverseXFlag_ = false;
					}
				}
				if (name == "y")
				{
					pawn->pos_.y += static_cast<float>(std::atof(atr->value())) * pawn->delta_;
				}
			}
			return true;
		}
	};

	struct Fall
	{
		bool operator()(Pawn* pawn, rapidxml::xml_node<>* node)
		{
			pawn->pos_.y += pawn->yaddPower_;
			pawn->yaddPower_ += 0.7f;
			if (pawn->yaddPower_ < 0)
			{
				pawn->SetAnimation(Char_Anim_ID::Jump);
			}
			else
			{
				pawn->SetAnimation(Char_Anim_ID::Fall);
			}
			return true;
		}
	};

	struct Jump
	{
		bool operator()(Pawn* pawn, rapidxml::xml_node<>* node)
		{
			pawn->yaddPower_ = pawn->defJunpPower_;
			pawn->pos_.y += pawn->yaddPower_;
			return true;
		}
	};

	struct CheckKeyTrg
	{
		bool operator()(Pawn* pawn, rapidxml::xml_node<>* node)
		{
			auto atr = node->first_attribute("key");
			if (atr == nullptr)
			{
				//assert(!"”»•Ê‚·‚ékey‚Ìattribute‚ª‚È‚¢");
				return false;
			}
			std::string id = atr->value();
			if (input_IDkey_.count(id) <= 0)
			{
				//assert(!"strng‚É‘Î‰ž‚µ‚½InputID–¢“o˜^");
				return false;
			}
			if (pawn->controller_->GetTrg(input_IDkey_[id]))
			{
				return true;
			}
			return false;
		}
	};

	struct CheckKey
	{
		bool operator()(Pawn* pawn, rapidxml::xml_node<>* node)
		{
			auto atr = node->first_attribute("key");
			if (atr == nullptr)
			{
				//assert(!"”»•Ê‚·‚ékey‚Ìattribute‚ª‚È‚¢");
				return false;
			}
			std::string id = atr->value();
			if (input_IDkey_.count(id) <= 0)
			{
				//assert(!"strng‚É‘Î‰ž‚µ‚½InputID–¢“o˜^");
				return false;
			}
			if (pawn->controller_->GetNow(input_IDkey_[id]))
			{
				return true;
			}
			return false;
		}
	};

	struct Stop
	{
		bool operator()(Pawn* pawn, rapidxml::xml_node<>* node)
		{
			if (oldPos == pawn->pos_)
			{
				return true;
			}
			oldPos = pawn->pos_;
			return false;
		}
	private:
		Vector2Flt oldPos = {};
	};

	struct SetAnim
	{
		bool operator()(Pawn* pawn, rapidxml::xml_node<>* node)
		{
			auto atr = node->first_attribute("id");
			if (atr == nullptr)
			{
				//assert(!"”»•Ê‚·‚éid‚Ìattribute‚ª‚È‚¢");
				return false;
			}
			std::string id = atr->value();
			if (id == "Run")
			{
				pawn->SetAnimation(Char_Anim_ID::Run);
			}
			if (id=="Idle")
			{
				pawn->SetAnimation(Char_Anim_ID::Idle);
			}
			if (id == "death")
			{
				pawn->SetAnimation(Char_Anim_ID::DEATH);
			}
			return true;
		}
	};

	struct EndNode
	{
		bool operator()(Pawn* pawn, rapidxml::xml_node<>* node)
		{
			return false;
		}
	};

	struct ModuleNode
	{
		bool operator()(Pawn* pawn, rapidxml::xml_node<>* node)
		{
			for (auto module = node->first_node(); module != nullptr; module = module->next_sibling())
			{
				std::string moduleName = module->first_attribute("name")->value();
				if (module_.count(moduleName) <= 0)
				{
					//assert(!"state.xml‚ÅŽw’è‚³‚ê‚Ä‚¢‚émodule‚ª‘¶Ý‚µ‚È‚¢–”‚Í“o˜^‚³‚ê‚Ä‚¢‚È‚¢");
					return false;
				}
				if (!module_[moduleName](pawn, module))
				{
					continue;
				}
				// Ä‹NŒÄ‚Ño‚µ
				if (!(*this)(pawn, module))
				{
					return false;
				}
			}
			return true;
		}
		std::map<std::string, std::function<bool(Pawn* pawn, rapidxml::xml_node<>* node)>> module_ = {
			{"Move",Move()},
			{"CheckKeyTrg",CheckKeyTrg()},
			{"CheckKey",CheckKey()},
			{"SetAnim",SetAnim()},
			{"Stop",Stop()},
			{"CheckRay",CheckRay()},
			{"CheckG",CheckG()},
			{"Fall",Fall()},
			{"Jump",Jump()},
			{"CheckAnim",CheckAnim()},
			{"Attack",Attack()},
			{"Command",Command()},
			{"CheckAlive",CheckAlive()},
			{"EndNode",EndNode()}
		};
	};
}