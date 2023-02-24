#include "../../Resource/Tmx/TmxObj.h"
#include "Unit.h"

Unit::Unit(const Math::Vector2& pos, std::shared_ptr<TmxObj>& tmxObj) :
	tmx_{tmxObj},isIdle_{false}, Obj{pos}
{
}

Unit::~Unit()
{
}

void Unit::MovePos(float delta)
{
    MapCollMng::Ray ray{ pos_,moveVec_ * delta * speed_ * spMag_ };
    for (auto& c : tmx_->GetColList())
    {
        const auto& [flag, moveV, colV] = MapCollMng::CheckColl2(ray, c);
        if (flag)
        {
            if (Math::Dot(moveVec_, colV) > 0.0f)
            {
                auto val = 1.0f - (moveV.SqMagnitude() / Math::Square(speed_ * spMag_));
                moveVec_ = (moveV + (colV * val)).Normalized();
            }
            else
            {
                auto val = 1.0f - (moveV.SqMagnitude() / Math::Square(speed_ * spMag_));
                moveVec_ = (moveV + (-colV * val)).Normalized();
            }
            break;
        }
    }

    // 後でデルタタイム使ったものに変更予定
    pos_ += moveVec_ * delta * speed_ * spMag_;
}

const bool Unit::IsMove(float delta) const
{
    MapCollMng::Ray ray{ pos_,moveVec_  *speed_ * delta * spMag_ };
    for (auto& c : tmx_->GetColList())
    {
        if (MapCollMng::CheckCollision(ray, c).first)
        {
            return false;
        }
    }
    return true;
}
