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
    // �v���C���[�ɓ��܂ꂽ���̍��W��Ԃ�
    Math::Vector2 GetApos(void);
    // �x���Ԑ��ɂȂ邩�ǂ������f����
    bool GetAFlag(void);
    // �������傫���Ȃ邩�ǂ���
    bool GetSFlag(void);
private:
    ObjID GetObjID(void) override { return ObjID::SnareFloor; };
    std::unique_ptr<Squar> squar_;
    ObjManager& objMng_;
    bool flag_;
    float sTime_;
    // �����Ȃ鏰���x�񂪂Ȃ鏰�����擾(���݂�2������int�^��0,1�ł�邯�Ǒ�����Ȃ�ς���)
    int type_;
    // �v���C���[����ɏ�����u�Ԃ̍��W
    Math::Vector2 sPos_;
    // �v���C���[����������ǂ���(�x���Ԑ�)
    bool AFlag_;
    // �v���C���[����������ǂ���(�������傫���Ȃ�)
    bool isBeforGameScene_;
};

