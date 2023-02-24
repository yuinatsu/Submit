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
    /// <summary> コンストラクタ </summary>
    /// <param name="cntType"> コントローラ情報 </param>
    /// <param name="tmx"> TmxObjクラス </param>
    /// <param name="pos"> 初期座標 </param>
    Player(CntType cntType, std::shared_ptr<TmxObj> tmx, Math::Vector2 pos);
    ~Player();

private:
    /// <summary> 初期化 </summary>
    /// <param name="cntType"> コントローラーの種類 </param>
    /// <returns> 正常動作:true </returns>
    bool Init(CntType cntType);
    
    /// <summary> 更新 </summary>
    /// <param name="delta"> デルタタイム </param>
    void Update(float delta)override;

    /// <summary> 描画 </summary>
    /// <param name="offset"> オフセット </param>
    /// <param name="drawMng"> 描画Manager </param>
    void Draw(const Math::Vector2& offset, DrawMng& drawMng)override;

    /// <summary> ID取得 </summary>
    /// <returns> ObjID::Player </returns>
    ObjID GetObjID(void) override { return ObjID::Player; };

    /// <summary> プレイヤーアクション </summary>
    std::vector<char> stateVec_;
    rapidxml::xml_document<> stateDoc_;
    rapidxml::xml_node<>* stateNode_;
    std::unique_ptr<animNode> animNode_;

    /// <summary> コントローラ情報 </summary>
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

