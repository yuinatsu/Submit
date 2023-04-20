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
#include "../../Factory/FactoryID.h"
#include "ThrusterBehavior.h"
#include "SandSmokeBehavior.h"
#include "SandSmokeSlowMoveBehavior.h"
#include "SandSmokeDiffusionBehavior.h"

class PlayerBehavior :
    public Behavior
{
public:
    PlayerBehavior(void);

    ComponentID GetID(void) const override
    {
        return id_;
    }
    static constexpr ComponentID id_{ ComponentID::PlayerBehavior };

    // �Q�[�W���擾
    std::pair<float, std::pair<float, float>> GetGaugeValue(UiID id) const { return gauge_.at(id); }
    // �U�����擾
    int GetAtkCnt(void) { return atkCnt_; }
    // �R���{���擾
    int GetComboNum(void) { return combo_; }
    // �R���{�����Z
    void AddCombo(void) { combo_++; }
    // �X�L���Q�[�W�㏸
    void RiseSkillValue(void);
    // �X�L���U���I��
    void EndSkill(void) { isSkill_ = false; }
    // ���݉�𒆂�
    bool IsDodge(void) { return isDodge_; }
    // ���݃_�b�V������
    bool IsDash(void) { return isDash_; }
private:
    // ������
    bool Init(void);
    void Begin(ObjectManager& objectManager) final;
    // �X�V
    void Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller) final;
    // �ړ�����
    float ProcessMove(BaseScene& scene, ObjectManager& objectManager, Controller& controller, float speed, float delta);
    // �K�E�������Ɍ�ɉ�����
    void ProcessMoveSkill(float interval, float delta);
    // �_�b�V������
    float ProcessDash(Controller& controller, const float& delta);
    // ��]����
    void ProcessRotate(ObjectManager& objectManager, BaseScene& scene, Controller& controller, float rot);
    // �W�����v����
    void ProcessJump(Controller& controller);
    // ���V����
    void ProcessFloat(void);
    // �U������
    void ProcessAttack(Controller& controller, ObjectManager& objectManager);
    // �X�^�~�i����
    void ProcessStamina(Controller& controller);
    // �N�[���^�C���v��
    void CoolTimer(float& delta);
    // �X�L������
    void ProcessSkill(Controller& controller, ObjectManager& objectManager);
    // �A�j���[�V��������
    void Animation(Controller& controller, ObjectManager& objectManager);
    // ���֌W����
    void Sound(void);
    // �Փˏ���
    void OnHit(Collider& col, ObjectManager& objectManager);
    // �n�ʂ̍�������
    void CorrectionGroundPos(void);
    // �R���{���Z�b�g
    void ResetCombo(void);

    // ����p������
    static constexpr float DodgeLimit = 0.5f;
    // �X�^�~�i�؂�N�[���^�C��
    static constexpr float CoolTime = 3.0f;
    // ���X�ɉ�]�����
    static constexpr float SpeedRotRad = 5.0f * (DX_PI_F / 180.0f);
    // �Q�[�W�㏸�萔
    static constexpr float RiseValue = 0.25f;
    // �U���A�j���[�V�����ő�ԍ�
    static constexpr int AtkAnimMax = 3;
    // �z�o�����OY���W�ō��l
    static constexpr float HoverPosYMax = 1000.0f;
    // ���VY���W�ō��l
    static constexpr float FloatPosYMax = 8.0f;
    // �v���C���[�̉�����Offset�i���j
    static constexpr float playerDownOffset = 80.0f;

    // �ړ���
    struct moveDis {
        float walk = 10.0f;         // ����
        float dash = 30.0f;         // �_�b�V��
        float dodge = 80.0f;        // ���
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
    // ���V
    struct floating {
        float initVel = 10.0f;      // �����x
        float vel = 0.0f;           // y�����̑��x
        float acc = 0.01f;          // �d�͉����x
        float groundPosY = 0.0f;    // �n�ʂ�Y���W
        bool isUp = true;           // �㏸����
        bool canFloat = true;       // ���s�\��
    };
    // �A�j���[�V�����Đ�����
    struct animTime {
        float idle = 0.1f;          // ����
        float move = 0.1f;          // �ړ�
        float attack1 = 0.3f;       // 1����
        float attack2 = 0.1f;       // 2����
        float attack3 = 0.3f;       // 3����
        float skill = 0.5;
    };

    // �A�j���[�V����
    ComponentHandle<Animator> animation_;
    // �g�����X�t�H�[��
    ComponentHandle<Transform> transform_;
    // �R���W����
    ComponentHandle<CharactorCollider> collider_;

    // �u���X�^�[�G�t�F�N�g
    ComponentHandle<ThrusterBehavior> blaster_;
    // �����G�t�F�N�g
    ComponentHandle<SandSmokeBehavior> sand_;
    // �����i�L����j�G�t�F�N�g
    ComponentHandle<SandSmokeDiffusionBehavior> sandDiff_;
    // ����(�x�����p)
    ComponentHandle<SandSmokeSlowMoveBehavior> sandSlow_;


    // �J�����̃g�����X�t�H�[��
    ComponentHandle<Transform> camera_;

    // �ړ���
    moveDis movedis_;
    // �_�b�V�����n�߂���o�ߎ���
    float dodgeTimer_;
    // ��𒆂�
    bool isDodge_;
    // ���݃_�b�V������
    bool isDash_;
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
    // ���V
    floating float_;
    // �U������
    bool atkLimit_;
    // �U���A�j���[�V��������
    bool isAttack_;
    // �U����
    int atkCnt_;
    // �R���{��
    int combo_;
    // �X�L���g�p�\��
    bool canSkill_;
    // �X�L���g�p����
    bool isSkill_;

    // �A�j���[�V�������
    std::vector<std::pair<AnimIndex, float>> anim_;
    // �A�j���[�V�������Ƃ̍Đ�����
    animTime animtime_;

    // �Q�[�W�l���
    std::map<UiID, std::pair<float, std::pair<float, float>>> gauge_;

    // �T�E���h
    std::vector<std::pair<SOUNDNAME_SE, bool>> sound_;
};

