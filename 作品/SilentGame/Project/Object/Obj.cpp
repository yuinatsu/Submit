#include <algorithm>
#include "../Component/Component.h"
#include "Obj.h"
#include "../common/Debug.h"

#include "../common/MapCollMng.h"

Obj::Obj(const Math::Vector2& pos) :
    alive_{true}, pos_{pos}, delta_{0.0f}, speed_{0.0f},item_{},spMag_{1.0f}
{
    effect_ = EffectName::Max;
    isAtk_ = false;
}

Obj::~Obj()
{
}

void Obj::Update(float delta)
{
    UpdateComponent(delta);
}

//void Obj::Draw(void) const
//{
//}

void Obj::Draw(const Math::Vector2& offset, DrawMng& drawMng)
{
}

const Components& Obj::GetAllComponent(ComponentID id) const
{
    if (!componentList_.contains(id))
    {
        return (componentList_.begin()->second);
    }
    return componentList_.at(id);

}

Component& Obj::GetComponent(ComponentID id, int idx) const
{
    if (!componentList_.contains(id))
    {
        return *(*componentList_.begin()->second.begin());
    }
    return *componentList_.at(id).at(idx);
}

bool Obj::IncludeComponent(ComponentID id) const
{
    return componentList_.contains(id);
}

bool Obj::AddComponent(ComponentUPTr&& component)
{
    componentList_[component->GetID()].emplace_back(std::move(component));
    return true;
}


const Math::Vector2& Obj::GetPos(void) const
{
    return pos_;
}

void Obj::SetPos(const Math::Vector2& pos)
{
    pos_ = pos;
}

void Obj::SetMoveVec(const Math::Vector2& moveVec)
{
    moveVec_ = moveVec ;
}

const Math::Vector2& Obj::GetMoveVec(void) const
{
    return moveVec_;
}

void Obj::MovePos(float delta)
{
   // MapCollMng::Ray ray{ pos_,moveVec_ * speed_ * spMag_ };
   

    // 後でデルタタイム使ったものに変更予定
    pos_ += moveVec_ * (speed_ * delta * spMag_);
}

const bool Obj::IsMove(float delta) const
{
    return true;
}

const float Obj::GetSpeed(void) const
{
    return speed_;
}

void Obj::SetSpMag(float spMag)
{
    spMag_ = spMag;
}

const float Obj::GetSpMag(void) const
{
    return spMag_;
}

//void Obj::SetAnimState(std::string str)
//{
//    anim_->state(str);
//}

void Obj::UpdateComponent(float delta) const
{
    for (auto& comps : componentList_)
    {
        for (auto& comp : comps.second)
        {
            comp->Update(delta);
        }
    }
}
