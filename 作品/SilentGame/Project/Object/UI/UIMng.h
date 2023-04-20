#pragma once
#include <memory>
#include <list>
#include <vector>
#include <fstream>
#include "UiBase.h"
#include "../../common/DrawMng.h"
#include "../../common/Math.h"

class ObjManager;

class UIMng
{
public:
	// �R���X�g���N�^ 
	UIMng();
	~UIMng();

	// �X�V 
	bool Update(float delta, const std::shared_ptr<ObjManager> objMng);

	// �`�� 
	void Draw(const double& delta);

	// itemUI��shered_ptr�擾 
	std::shared_ptr<UiBase> GetItemUI() { return itemUI_; }

	// staminaUI��shered_ptr�擾 
	std::shared_ptr<UiBase> GetStaminaUI() { return staminaUI_; }

	// �J�n�����o�I��
	bool FinStart();
private:
	// �A�C�e���擾�Ǝg�p�A�C�e���̍X�V 
	bool UpdateGetUse(float delta, const std::shared_ptr<ObjManager> objMng);

	// �X�s�[�h�{���X�V 
	bool UpdateSpMag(const std::shared_ptr<ObjManager> objMng);

	// �A�C�e��UI 
	std::shared_ptr<UiBase> itemUI_;

	// �X�^�~�iUI 
	std::shared_ptr<UiBase> staminaUI_;

	// �X�^�~�i����t���O 
	bool stanimaF_;

	// �G�t�F�N�g�t���O
	bool effectF_;
};

