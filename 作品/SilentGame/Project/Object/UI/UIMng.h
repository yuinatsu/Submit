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
	/// <summary> �R���X�g���N�^ </summary>
	UIMng();
	~UIMng();

	/// <summary> �X�V </summary>
	/// <param name="delta"> �f���^�^�C�� </param>
	/// <param name="objMng"> Obj�N���X��Manager </param>
	/// <returns> ���퓮��:true </returns>
	bool Update(float delta, const std::shared_ptr<ObjManager> objMng);

	/// <summary> �`�� </summary>
	/// <param name="delta"> �f���^�^�C�� </param>
	void Draw(const double& delta);

	/// <summary> itemUI��shered_ptr�擾 </summary>
	/// <returns> itemUI_ </returns>
	std::shared_ptr<UiBase> GetItemUI() { return itemUI_; }

	/// <summary> staminaUI��shered_ptr�擾 </summary>
	/// <returns> staminaUI_ </returns>
	std::shared_ptr<UiBase> GetStaminaUI() { return staminaUI_; }

	bool FinStart();
private:
	/// <summary> �A�C�e���擾�Ǝg�p�A�C�e���̍X�V </summary>
	/// <param name="delta"> �f���^�^�C�� </param>
	/// <param name="objMng"> Obj�N���X��Manager </param>
	/// <returns> ���퓮��:true </returns>
	bool UpdateGetUse(float delta, const std::shared_ptr<ObjManager> objMng);

	/// <summary> �X�s�[�h�{���X�V </summary>
	/// <param name="objMng"> Obj�N���X��Manager </param>
	/// <returns> ���퓮��:true </returns>
	bool UpdateSpMag(const std::shared_ptr<ObjManager> objMng);

	/// <summary> �A�C�e��UI </summary>
	std::shared_ptr<UiBase> itemUI_;

	/// <summary> �X�^�~�iUI </summary>
	std::shared_ptr<UiBase> staminaUI_;

	/// <summary> �X�^�~�i����t���O </summary>
	bool stanimaF_;

	bool effectF_;
};

