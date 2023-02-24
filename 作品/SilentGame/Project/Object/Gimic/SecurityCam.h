#pragma once
#include <list>
#include "../../common/MapCollMng.h"
#include "../../common/Math.h"
#include "../../Resource/Tmx/TmxObj.h"
#include "../../common/Camera/Camera.h"
#include "../../common/Structure.h"
#include "../../common/Time.h"
#include "GimicObj.h"

class Collider;

class Alert:
	public GimicObj
{
public:
	Alert(std::weak_ptr< GmkEvent>& gmkEvent,Math::Vector2 pos, Math::Vector2 vec, std::shared_ptr<TmxObj>& tmxObj, std::weak_ptr<Obj>&& player);
	~Alert();
	void Init(void);
	void Update(float delta) override;
	// �v���C���[�������Ă��Ȃ��Ƃ��̏���
	void UpdateNoDis(const Math::Vector2& pos, float delta);
	// �v���C���[�����������̏���
	void UpdateDis(const Math::Vector2& pos, float delta);
	void Draw(const Math::Vector2& offset, DrawMng& drawMng) override;
	ObjID GetObjID(void) { return ObjID::Alert; };
	
	// �v���C���[�����m�������ǂ������󂯎��
	bool GetpFlag(void);

private:

	void Hit(Collider& other);

	// �}�b�v�����󂯎��
	std::shared_ptr<TmxObj> tmxObj_;
	// �v���C���[�̏����󂯎��
	std::weak_ptr<Obj> player_;

	// �����̉�]���t��]��
	//bool rotFlag_;
	// �����̉�]�𔽓]����܂ł̎���
	//float rotTime_;
	// �v���C���[���������ǂ���
	bool pflag_;

	//std::unique_ptr<Fan> fan_;

	// �v���C���[���ǂꂩ�̎����ɓ��������ǂ���
	bool sFlag_;
	// �o�ߎ���
	float time_;

	// ���E�ǂ������ړ]��
	float sig_;

	// ��~�t���O
	bool stopFlag_;
};

