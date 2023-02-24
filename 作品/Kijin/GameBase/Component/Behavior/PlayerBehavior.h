#pragma once
#include <map>
#include <string>
#include <vector>
#include "../../Object/ObjectManager.h"
#include "../Transform/Transform.h"
#include "../Collider/CharactorCollider.h"
#include "../../Common/Input/Controller.h"
#include "Behavior.h"
#include "../Render/ModelRender.h"
#include "../Animator/Animator.h"
#include "../../UI/UiID.h"
#include "../../Common/SoundPross.h"
#include "../Animator/PlayerAnimIndex.h"

class PlayerBehavior :
    public Behavior
{
public:
    PlayerBehavior(CntType cnt);

    ComponentID GetID(void) const override
    {
        return id_;
    }
    static constexpr ComponentID id_{ ComponentID::PlayerBehavior };

    // �X�L���Q�[�W�㏸
    void RiseSkillValue(void);
    // �Q�[�W���擾
    std::pair<float, std::pair<float, float>> GetGaugeValue(UiID id) const;
    // �R���{���擾
    int GetComboNum(void) { return combo_; }
    // �R���{�����Z
    void AddCombo(void) { combo_++; }
private:
    // ������
    bool Init(CntType cnt);
    void Begin(ObjectManager& objectManager) final;
    // �X�V
    void Update(BaseScene& scene,ObjectManager& objectManager, float delta, Controller& controller) final;
    // �ړ�����
    float ProcessMove(BaseScene& scene,Controller& controller, float speed);
    // �_�b�V������
    float ProcessDash(Controller& controller,const float& delta);
    // ��]����
    void ProcessRotate(BaseScene& scene,Controller& controller, float rot);
    // �W�����v����
    void ProcessJump(Controller& controller);
    // �U������
    void ProcessAttack(Controller& controller, ObjectManager& objectManager);
    // �X�^�~�i����
    void ProcessStamina(Controller& controller);
    // �N�[���^�C���v��
    void CoolTimer(float& delta);
    // �X�L������
    void ProcessSkill(Controller& controller);
    // �A�j���[�V��������
    void Animation(Controller& controller);
    // ���֌W����
    void Sound(Controller& controller);
    // �Փˏ���
    void OnHit(Collider& col, ObjectManager& objectManager);

    // ����p������
    static constexpr float DodgeLimit = 0.2f;
    // �X�^�~�i�؂�N�[���^�C��
    static constexpr float CoolTime = 3.0f;
    // ���X�ɉ�]�����
    static constexpr float SpeedRotRad = 5.0f * (DX_PI_F / 180.0f);
    // �Q�[�W�㏸�萔
    static constexpr float RiseValue = 0.25f;
    // �U���A�j���[�V�����ő�ԍ�
    static constexpr int AtkAnimMax = 3;

    // �ړ���
    struct moveDis {
        float walk = 10.0f;          // ����
        float dash = 30.0f;         // �_�b�V��
        float dodge = 75.0f;        // ���
    };
    // �����p�x
    struct dir {
        float left = 90.0f;         // ��
        float right = 270.0f;       // �E
        float up = 180.0f;          // ��
        float down = 0.0f;          // ��
    };
    // �W�����v
    struct jump {
        float initVel = 10.0f;      // �����x
        float vel = 0.0f;           // y�����̑��x
        float acc = 1.0f;           // �d�͉����x
    };
    // �A�j���[�V�����Đ�����
    struct animTime {
        float idle = 0.1f;          // ����
        float move = 0.1f;          // �ړ�
        float attack1 = 0.3f;       // 1����
        float attack2 = 0.1f;       // 2����
        float attack3 = 0.3f;       // 3����
    };

    // �A�j���[�V����
    ComponentHandle<Animator> animation_;
    // �g�����X�t�H�[��
    ComponentHandle<Transform> transform_;
    // �R���W����
    ComponentHandle<CharactorCollider> collider_;

    // �ړ���
    moveDis movedis_;
    // �_�b�V�����n�߂���o�ߎ���
    float dodgeTimer_;
    // ��𒆂�
    bool isDodge_;
    // �_�b�V���֎~����
    float noDashTime_;
    // �X�^�~�i�؂�ɂȂ�����
    bool isStaminaLoss_;
    // �����p�x
    dir dir_;
    // ��]���
    Vector3 angles_;
    // �W�����v
    jump jump_;

    // �U������
    bool isAttack_;
    // �U���A�j���[�V�����ԍ�
    int atkAnim_;
    // �R���{��
    int combo_;
    // �X�L���g�p�\��
    bool useSkill_;

    // �A�j���[�V�������
    std::vector<std::pair<AnimIndex, float>> anim_;
    // �A�j���[�V�������Ƃ̍Đ�����
    animTime animtime_;

    // �Q�[�W�l���
    std::map<UiID, std::pair<float, std::pair<float, float>>> gauge_;

    // �T�E���h
    std::vector< std::pair<SoundProcess::SOUNDNAME_SE, bool>> sound_;
};

