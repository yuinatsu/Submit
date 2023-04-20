#pragma once
#include <map>
#include <list>
#include <sstream>
#include "Unit.h"
#include "../../common/rapidxml_utils.hpp"

struct animNode;

class Player :
    public Unit
{
public:
    //　コンストラクタ　
    Player(CntType cntType, std::shared_ptr<TmxObj> tmx, Math::Vector2 pos);
    ~Player();

private:
    //　初期化　
    bool Init(CntType cntType);
    
    //　更新　
    void Update(float delta)override;

    //　描画　
    void Draw(const Math::Vector2& offset, DrawMng& drawMng)override;

    //　ID取得　
    ObjID GetObjID(void) override { return ObjID::Player; };

    //　プレイヤーアクション　
    std::vector<char> stateVec_;
    rapidxml::xml_document<> stateDoc_;
    rapidxml::xml_node<>* stateNode_;
    std::unique_ptr<animNode> animNode_;

    //　コントローラ情報　
    std::unique_ptr<Controller> controller_;

    // フレンド
    friend struct CheckKey;
    friend struct Attack;

    std::string spMagName_;

    float oldSpMag_;

    // 攻撃してからの経過時間
    float attackStepTime_;

    int cBuff;
    float* threshold;
};

