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
	/// <summary> コンストラクタ </summary>
	UIMng();
	~UIMng();

	/// <summary> 更新 </summary>
	/// <param name="delta"> デルタタイム </param>
	/// <param name="objMng"> ObjクラスのManager </param>
	/// <returns> 正常動作:true </returns>
	bool Update(float delta, const std::shared_ptr<ObjManager> objMng);

	/// <summary> 描画 </summary>
	/// <param name="delta"> デルタタイム </param>
	void Draw(const double& delta);

	/// <summary> itemUIのshered_ptr取得 </summary>
	/// <returns> itemUI_ </returns>
	std::shared_ptr<UiBase> GetItemUI() { return itemUI_; }

	/// <summary> staminaUIのshered_ptr取得 </summary>
	/// <returns> staminaUI_ </returns>
	std::shared_ptr<UiBase> GetStaminaUI() { return staminaUI_; }

	bool FinStart();
private:
	/// <summary> アイテム取得と使用アイテムの更新 </summary>
	/// <param name="delta"> デルタタイム </param>
	/// <param name="objMng"> ObjクラスのManager </param>
	/// <returns> 正常動作:true </returns>
	bool UpdateGetUse(float delta, const std::shared_ptr<ObjManager> objMng);

	/// <summary> スピード倍率更新 </summary>
	/// <param name="objMng"> ObjクラスのManager </param>
	/// <returns> 正常動作:true </returns>
	bool UpdateSpMag(const std::shared_ptr<ObjManager> objMng);

	/// <summary> アイテムUI </summary>
	std::shared_ptr<UiBase> itemUI_;

	/// <summary> スタミナUI </summary>
	std::shared_ptr<UiBase> staminaUI_;

	/// <summary> スタミナ動作フラグ </summary>
	bool stanimaF_;

	bool effectF_;
};

