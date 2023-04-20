#pragma once
#include <memory>
#include <vector>
#include <string>
#include <functional>
#include "../Item/ItemName.h"
#include "../../common/Math.h"
#include "../ObjManager.h"

// �ő�A�C�e�������� 
constexpr int haveItemMax = 6;

// pair<ID, �A�C�e����> 
using IdData = std::pair<int, std::string>;

// UI�̎�� 
enum class UIID
{
	ItemUI,				// �����A�C�e��
	StaminaUI,			// �X�^�~�i
	Max
};

class Controller;

class UiBase
{
public:
	// �R���X�g���N�^ 
	UiBase(const Math::Vector2& pos, const Math::Vector2& UISize);
	virtual ~UiBase() = default;

	// �X�V 
	virtual void Update(const double& delta, const std::shared_ptr<ObjManager> objMng) = 0;

	virtual void UpdateStart(const double& delta, const std::shared_ptr<ObjManager> objMng) = 0;

	virtual void UpdateGame(const double& delta, const std::shared_ptr<ObjManager> objMng) = 0;

	// �A�C�e�����X�V 
	void UpdateItem(const double& delta);

	// UI�����`�� 
	virtual void DrawUI(const double& delta) = 0;

	virtual void DrawStart(const double& delta) = 0;

	virtual void DrawGame(const double& delta) = 0;

	// �A�C�e��ID�Z�b�g 
	bool SetItemID(IdData id);

	// �A�C�e��ID�擾 
    std::vector<IdData> GetItemID() { return id_; };

	// �g�p���ꂽ�A�C�e�����擾 
	ItemName GetUseItem();

	// �����t���O 
	bool isAlive(void) { return isAlive_; };

	// ID�擾 
	virtual UIID GetUIID(void) = 0;

	// �X�^�~�i�l�擾 
	float GetStamina(void) { return stamina_; }

	// �X�s�[�h�{���Z�b�g 
	void SetSpMag(float mag) { spMag_ = mag; }

	// �Q�[�W�ő�l�擾
	int GetGageMaxCnt(void) { return gageMaxCnt_; }

	virtual bool IsStartFin() = 0;
protected:
	// UI���W 
	Math::Vector2 pos_;

	// UI�T�C�Y 
	Math::Vector2 UISize_;

	// ���S�t���O 
	bool isAlive_;

	// �����A�C�e���ꗗ 
	std::vector<std::pair<int, bool>> order_;

	// �R���g���[���[��� 
	std::unique_ptr<Controller> controller_;

	// �A�C�e��ID 
	std::vector<IdData> id_;

	// �g�p���ꂽ�A�C�e�� 
	ItemName useItem_;

	// �}�X�N��� 
	int mask_;

	// �X�^�~�i�l 
	float stamina_;

	// �X�s�[�h�{�� 
	float spMag_;

	// �Q�[�W���������ő�l
	int gageMaxCnt_;

	// �Q�[�������ԃJ�E���^
	float count_;

	// �؂�ւ��p
	std::function<void(float, const std::shared_ptr<ObjManager>)> updateFunc_;
	std::function<void(double)> drawFunc_;
};

