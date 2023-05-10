#pragma once
#include "Behavior.h"
#include "../Transform/Transform.h"
#include "../ComponentHandle.h"

class CameraBehavior :
    public Behavior
{
public:

	enum class Type
	{
		Sync,
		Non,
	};

	ComponentID GetID(void) const final
	{
		return id_;
	}
	static constexpr ComponentID id_{ ComponentID::CameraBehavior };

	/// <summary>
	/// �J�����̃V�F�C�N���J�n����
	/// </summary>
	/// <param name="shakeTime"> �V�F�C�N���鎞�� </param>
	void StartShake(float shakeTime);

	/// <summary>
	/// ��������I�u�W�F�N�g��ID���Z�b�g����
	/// </summary>
	/// <param name="id"></param>
	void SetSyncID(ObjectID& id)
	{
		syncID_ = id;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="speed"></param>
	void SetSpeed(float speed);

	void SetType(Type type);
private:

	void Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller) final;
	void Begin(ObjectManager& objectManager) final;

	// �X�V����
	void (CameraBehavior::* update_)(BaseScene&, ObjectManager&, float, Controller&);

	/// <summary>
	/// �I�u�W�F�N�g�Ɠ������鎞�̍X�V����
	/// </summary>
	/// <param name="scene">  </param>
	/// <param name="objectManager"></param>
	/// <param name="delta"></param>
	/// <param name="controller"></param>
	void UpdateSync(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller);

	/// <summary>
	/// �������Ȃ��Ƃ��p�̍X�V����
	/// </summary>
	/// <param name="scene"></param>
	/// <param name="objectManager"></param>
	/// <param name="delta"></param>
	/// <param name="controller"></param>
	void UpdateNon(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller);

	/// <summary>
	/// �J�����V�F�C�N����
	/// </summary>
	/// <param name="delta"></param>
	void Shake(float delta);

	/// <summary>
	/// �J�����I�t�Z�b�g
	/// </summary>
	/// <param name="delta"></param>
	void CameraOffset(float delta);

	/// <summary>
	/// �o�ߎ���
	/// </summary>
	float stepTime_;

	/// <summary>
	/// ���g�̃g�����X�t�H�[��
	/// </summary>
	ComponentHandle<Transform> transform_;

	/// <summary>
	/// ������̃I�u�W�F�N�gID
	/// </summary>
	ObjectID syncID_;

	/// <summary>
	/// �V�F�C�N�O�̍��W
	/// </summary>
	Vector3 defPos_;

	/// <summary>
	/// �J�������V�F�C�N���Ă��邩
	/// </summary>
	bool isShake_;

	/// <summary>
	/// �J�������V�F�C�N���鎞��
	/// </summary>
	float shakeTime_;

	/// <summary>
	/// �J��������X�s�[�h
	/// </summary>
	float speed_;

	/// <summary>
	/// �_�b�V�����̃J�������W�I�t�Z�b�g
	/// </summary>
	float offset_;

	/// <summary>
	/// �I�t�Z�b�g�p�J�E���^
	/// </summary>
	float cnt_;

	/// <summary>
	/// �I�t�Z�b�g�̍ő�l
	/// </summary>
	float offsetMax_;

	Type nowType_;
};

