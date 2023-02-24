#pragma once
#include "../Obj.h"

class TmxObj;

class Unit :
    public Obj
{
public:
    Unit(const Math::Vector2& pos,std::shared_ptr<TmxObj>& tmxObj);
    virtual ~Unit();

    /// <summary> アイドル状態フラグをセットする </summary>
    /// <param name="flag"> セットするフラグ </param>
    void SetIdleFlag(bool flag) { isIdle_ = flag; }

    /// <summary> アイドル状態か？ </summary>
    /// <returns> アイドル状態の時trueそうでないときfalse </returns>
    const bool IsIdle(void)const { return isIdle_; }

    void MovePos(float delta) override;

    const bool IsMove(float delta) const override;

    virtual void CorrectMoveVecToAnim(void){}
protected:

    /// <summary> TmxObj情報 </summary>
    std::shared_ptr<TmxObj> tmx_;

    /// <summary> Idle状態かどうか </summary>
    bool isIdle_;


    friend struct MovePos;
    friend struct ColisionCheck;
};

