#pragma once
#include "GimicObj.h"
#include "../../common/Math.h"

struct Squar;
class ObjManager;

class SnareFloor :
    public GimicObj
{
public:
    SnareFloor(std::weak_ptr<GmkEvent>& gmkEvent, const Math::Vector2& pos, ObjManager& objMng,int Type);
    ~SnareFloor();
    void Init();
    void Update(float delta) override;
    void UpdataAlerm(float delta);
    void UpdataSound(float delta);
    void Draw(const Math::Vector2& offset, DrawMng& drawMng) override;
    // プレイヤーに踏まれた時の座標を返す
    Math::Vector2 GetApos(void);
    // 警戒態勢になるかどうか判断する
    bool GetAFlag(void);
    // 足音が大きくなるかどうか
    bool GetSFlag(void);
private:
    ObjID GetObjID(void) override { return ObjID::SnareFloor; };
    std::unique_ptr<Squar> squar_;
    ObjManager& objMng_;
    bool flag_;
    float sTime_;
    // 音がなる床か警報がなる床かを取得(現在は2つだからint型で0,1でやるけど増えるなら変える)
    int type_;
    // プレイヤーが上に乗った瞬間の座標
    Math::Vector2 sPos_;
    // プレイヤーが乗ったかどうか(警戒態勢)
    bool AFlag_;
    // プレイヤーが乗ったかどうか(足音が大きくなる)
    bool isBeforGameScene_;
};

