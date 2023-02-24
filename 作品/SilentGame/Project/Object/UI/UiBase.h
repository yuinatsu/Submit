#pragma once
#include <memory>
#include <vector>
#include <string>
#include <functional>
#include "../Item/ItemName.h"
#include "../../common/Math.h"
#include "../ObjManager.h"

/// <summary> �ő�A�C�e�������� </summary>
constexpr int haveItemMax = 6;

/// <summary> pair<ID, �A�C�e����> </summary>
using IdData = std::pair<int, std::string>;

/// <summary> UI�̎�� </summary>
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
	/// <summary> �R���X�g���N�^	 </summary>
	/// <param name="pos"> UI���W </param>
	/// <param name="UISize"> UI�T�C�Y </param>
	UiBase(const Math::Vector2& pos, const Math::Vector2& UISize);
	virtual ~UiBase() = default;

	/// <summary> �X�V </summary>
	/// <param name="delta"> �f���^�^�C�� </param>
	/// <param name="objMng"> Obj�N���X��Manager </param>
	virtual void Update(const double& delta, const std::shared_ptr<ObjManager> objMng) = 0;

	virtual void UpdateStart(const double& delta, const std::shared_ptr<ObjManager> objMng) = 0;

	virtual void UpdateGame(const double& delta, const std::shared_ptr<ObjManager> objMng) = 0;

	/// <summary> �A�C�e�����X�V </summary>
	/// <param name="delta"> �f���^�^�C�� </param>
	void UpdateItem(const double& delta);

	/// <summary> UI�����`�� </summary>
	/// <param name="delta"> �f���^�^�C�� </param>
	virtual void DrawUI(const double& delta) = 0;

	virtual void DrawStart(const double& delta) = 0;

	virtual void DrawGame(const double& delta) = 0;

	/// <summary> �A�C�e��ID�Z�b�g </summary>
	/// <param name="id"> IdData(ID,�A�C�e����) </param>
	/// <returns> �A�C�e���Z�b�g�\:true�A�s��:false </returns>
	bool SetItemID(IdData id);

	/// <summary> �A�C�e��ID�擾 </summary>
    /// <returns> Id_ </returns>
    std::vector<IdData> GetItemID() { return id_; };

	/// <summary> �g�p���ꂽ�A�C�e�����擾 </summary>
	/// <returns> �A�C�e���� </returns>
	ItemName GetUseItem();

	/// <summary> �����t���O </summary>
	/// <returns> ����:true�A���S:false </returns>
	bool isAlive(void) { return isAlive_; };

	/// <summary> ID�擾 </summary>
	virtual UIID GetUIID(void) = 0;

	/// <summary> �X�^�~�i�l�擾 </summary>
	/// <returns> �X�^�~�i�l </returns>
	float GetStamina(void) { return stamina_; }

	/// <summary> �X�s�[�h�{���Z�b�g </summary>
	/// <param name="mag"> �{�� </param>
	void SetSpMag(float mag) { spMag_ = mag; }

	int GetGageMaxCnt(void) { return gageMaxCnt_; }

	virtual bool IsStartFin() = 0;
protected:
	/// <summary> UI���W </summary>
	Math::Vector2 pos_;

	/// <summary> UI�T�C�Y </summary>
	Math::Vector2 UISize_;

	/// <summary> ���S�t���O </summary>
	bool isAlive_;

	/// <summary> �����A�C�e���ꗗ </summary>
	/// <param name="int"> �A�C�e��ID </param>
	/// <param name="bool"> �擪���ǂ��� </param>
	std::vector<std::pair<int, bool>> order_;

	/// <summary> �R���g���[���[��� </summary>
	std::unique_ptr<Controller> controller_;

	/// <summary> �A�C�e��ID </summary>
	std::vector<IdData> id_;

	/// <summary> �g�p���ꂽ�A�C�e�� </summary>
	ItemName useItem_;

	/// <summary> �}�X�N��� </summary>
	int mask_;

	/// <summary> �X�^�~�i�l </summary>
	float stamina_;

	/// <summary> �X�s�[�h�{�� </summary>
	float spMag_;

	// �Q�[�W���������ő�l
	int gageMaxCnt_;

	float count_;

	std::function<void(float, const std::shared_ptr<ObjManager>)> updateFunc_;
	std::function<void(double)> drawFunc_;
};

