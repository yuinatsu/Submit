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
    //�@�R���X�g���N�^�@
    Player(CntType cntType, std::shared_ptr<TmxObj> tmx, Math::Vector2 pos);
    ~Player();

private:
    //�@�������@
    bool Init(CntType cntType);
    
    //�@�X�V�@
    void Update(float delta)override;

    //�@�`��@
    void Draw(const Math::Vector2& offset, DrawMng& drawMng)override;

    //�@ID�擾�@
    ObjID GetObjID(void) override { return ObjID::Player; };

    //�@�v���C���[�A�N�V�����@
    std::vector<char> stateVec_;
    rapidxml::xml_document<> stateDoc_;
    rapidxml::xml_node<>* stateNode_;
    std::unique_ptr<animNode> animNode_;

    //�@�R���g���[�����@
    std::unique_ptr<Controller> controller_;

    // �t�����h
    friend struct CheckKey;
    friend struct Attack;

    std::string spMagName_;

    float oldSpMag_;

    // �U�����Ă���̌o�ߎ���
    float attackStepTime_;

    int cBuff;
    float* threshold;
};

