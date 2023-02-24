#include <DxLib.h>
#include <cmath>
#include "Pawn.h"
#include "Bullet.h"
#include "../Mng/AnimationMng.h"
#include "../Mng/ImageMng.h"
#include "../common/Raycast.h"
#include "../Input/Controller.h"
#include "../Input/KeyBoard.h"
#include "../Input/Pad.h"
#include "state/state.h"
#include "../Command/LoadCommand.h"
#include "../../_debug/_DebugConOut.h"


Pawn::Pawn(const Vector2Flt& pos, const Vector2& size, const Object_ID oID, int hp, TeamTag tag, InputType inputType) :
	Object(pos, size, oID,hp,tag)
{
    animID_=Char_Anim_ID::Idle;
    animCnt_ = 0;
    animLoopCnt_ = 0;
    moduleNode_ = std::make_unique<state::ModuleNode>();
    LoadCommad loadCommand;
    commandList_ = loadCommand("Resource/command.xml");
    for (const auto& data:commandList_)
    {
        //TRACE((data.name_ + "\n").c_str());
        for (const auto& com:data.command_)
        {
            if (!com.required_)
            {
                continue;
            }
            if (com.id == CMD_ID::LEFT)
            {
                //TRACE("LEFT:");
            }
            if (com.id == CMD_ID::RIGHT)
            {
                //TRACE("RIGHT:");
            }
            if (com.id == CMD_ID::UP)
            {
                //TRACE("UP:");
            }
            if (com.id == CMD_ID::DOWN)
            {
                //TRACE("DOWN:");
            }
            if (com.id == CMD_ID::LEFT_U_D)
            {
                //TRACE("LEFT_U_D:");
            }
            if (com.id == CMD_ID::LEFT_D_D)
            {
                //TRACE("LEFT_D_D:");
            }
            if (com.id == CMD_ID::RIGHT_U_D)
            {
                //TRACE("RIGHT_U_D:");
            }
            if (com.id == CMD_ID::RIGHT_D_D)
            {
                //TRACE("RIGHT_D_D:");
            }
            if (com.id == CMD_ID::SWT3)
            {
                //TRACE("BTN_3:");
            }
            if (com.id == CMD_ID::NEUTRAL)
            {
                //TRACE("NEUTRAL:");
            }
            //TRACE("%d", com.time);
            //TRACE("\n");
        }
    }
}

void Pawn::Draw(const double& delta)
{
    if (invincibleCnt_ > 0.0 && isAlive_)
    {
        if (static_cast<int>(std::floor(invincibleCnt_ * 10.0)) % 2 == 0)
        {
            DrawRotaGraph(static_cast<int>(pos_.x), static_cast<int>(pos_.y), exRate_, angle_, lpAnimMng.GetAnimImag(objectID_, animID_, animCnt_, animLoopCnt_), true, reverseXFlag_);
        }
    }
    else
    {
        DrawRotaGraph(static_cast<int>(pos_.x), static_cast<int>(pos_.y), exRate_, angle_, lpAnimMng.GetAnimImag(objectID_, animID_, animCnt_, animLoopCnt_), true, reverseXFlag_);
    }
    invincibleCnt_ -= delta;
    if (bullet_)
    {
        bullet_->Draw(delta);
    }
}

void Pawn::AddDamage(int damage)
{
    if (invincibleCnt_ < 0.0)
    {
        invincibleCnt_ = 2.0;
        hp_ -= damage;
        if (hp_ <= 0)
        {
            isAlive_ = false;
        }
    }
}


void Pawn::SetAnimation(Char_Anim_ID id)
{
    if (animID_ != id)
    {
        animID_ = id;
        animCnt_ = 0;
        animLoopCnt_ = 0;
    }
}
