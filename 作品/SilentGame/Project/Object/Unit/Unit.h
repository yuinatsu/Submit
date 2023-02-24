#pragma once
#include "../Obj.h"

class TmxObj;

class Unit :
    public Obj
{
public:
    Unit(const Math::Vector2& pos,std::shared_ptr<TmxObj>& tmxObj);
    virtual ~Unit();

    /// <summary> �A�C�h����ԃt���O���Z�b�g���� </summary>
    /// <param name="flag"> �Z�b�g����t���O </param>
    void SetIdleFlag(bool flag) { isIdle_ = flag; }

    /// <summary> �A�C�h����Ԃ��H </summary>
    /// <returns> �A�C�h����Ԃ̎�true�����łȂ��Ƃ�false </returns>
    const bool IsIdle(void)const { return isIdle_; }

    void MovePos(float delta) override;

    const bool IsMove(float delta) const override;

    virtual void CorrectMoveVecToAnim(void){}
protected:

    /// <summary> TmxObj��� </summary>
    std::shared_ptr<TmxObj> tmx_;

    /// <summary> Idle��Ԃ��ǂ��� </summary>
    bool isIdle_;


    friend struct MovePos;
    friend struct ColisionCheck;
};

