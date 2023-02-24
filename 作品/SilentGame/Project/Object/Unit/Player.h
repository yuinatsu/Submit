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
    /// <summary> �R���X�g���N�^ </summary>
    /// <param name="cntType"> �R���g���[����� </param>
    /// <param name="tmx"> TmxObj�N���X </param>
    /// <param name="pos"> �������W </param>
    Player(CntType cntType, std::shared_ptr<TmxObj> tmx, Math::Vector2 pos);
    ~Player();

private:
    /// <summary> ������ </summary>
    /// <param name="cntType"> �R���g���[���[�̎�� </param>
    /// <returns> ���퓮��:true </returns>
    bool Init(CntType cntType);
    
    /// <summary> �X�V </summary>
    /// <param name="delta"> �f���^�^�C�� </param>
    void Update(float delta)override;

    /// <summary> �`�� </summary>
    /// <param name="offset"> �I�t�Z�b�g </param>
    /// <param name="drawMng"> �`��Manager </param>
    void Draw(const Math::Vector2& offset, DrawMng& drawMng)override;

    /// <summary> ID�擾 </summary>
    /// <returns> ObjID::Player </returns>
    ObjID GetObjID(void) override { return ObjID::Player; };

    /// <summary> �v���C���[�A�N�V���� </summary>
    std::vector<char> stateVec_;
    rapidxml::xml_document<> stateDoc_;
    rapidxml::xml_node<>* stateNode_;
    std::unique_ptr<animNode> animNode_;

    /// <summary> �R���g���[����� </summary>
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

