#pragma once
#include <memory>
#include <vector>
#include <string>
#include <functional>
#include "../Item/ItemName.h"
#include "../../common/Math.h"
#include "../ObjManager.h"

/// <summary> 最大アイテム所持数 </summary>
constexpr int haveItemMax = 6;

/// <summary> pair<ID, アイテム名> </summary>
using IdData = std::pair<int, std::string>;

/// <summary> UIの種類 </summary>
enum class UIID
{
	ItemUI,				// 所持アイテム
	StaminaUI,			// スタミナ
	Max
};

class Controller;

class UiBase
{
public:
	/// <summary> コンストラクタ	 </summary>
	/// <param name="pos"> UI座標 </param>
	/// <param name="UISize"> UIサイズ </param>
	UiBase(const Math::Vector2& pos, const Math::Vector2& UISize);
	virtual ~UiBase() = default;

	/// <summary> 更新 </summary>
	/// <param name="delta"> デルタタイム </param>
	/// <param name="objMng"> ObjクラスのManager </param>
	virtual void Update(const double& delta, const std::shared_ptr<ObjManager> objMng) = 0;

	virtual void UpdateStart(const double& delta, const std::shared_ptr<ObjManager> objMng) = 0;

	virtual void UpdateGame(const double& delta, const std::shared_ptr<ObjManager> objMng) = 0;

	/// <summary> アイテム情報更新 </summary>
	/// <param name="delta"> デルタタイム </param>
	void UpdateItem(const double& delta);

	/// <summary> UI部分描画 </summary>
	/// <param name="delta"> デルタタイム </param>
	virtual void DrawUI(const double& delta) = 0;

	virtual void DrawStart(const double& delta) = 0;

	virtual void DrawGame(const double& delta) = 0;

	/// <summary> アイテムIDセット </summary>
	/// <param name="id"> IdData(ID,アイテム名) </param>
	/// <returns> アイテムセット可能:true、不可:false </returns>
	bool SetItemID(IdData id);

	/// <summary> アイテムID取得 </summary>
    /// <returns> Id_ </returns>
    std::vector<IdData> GetItemID() { return id_; };

	/// <summary> 使用されたアイテムを取得 </summary>
	/// <returns> アイテム名 </returns>
	ItemName GetUseItem();

	/// <summary> 生存フラグ </summary>
	/// <returns> 生存:true、死亡:false </returns>
	bool isAlive(void) { return isAlive_; };

	/// <summary> ID取得 </summary>
	virtual UIID GetUIID(void) = 0;

	/// <summary> スタミナ値取得 </summary>
	/// <returns> スタミナ値 </returns>
	float GetStamina(void) { return stamina_; }

	/// <summary> スピード倍率セット </summary>
	/// <param name="mag"> 倍率 </param>
	void SetSpMag(float mag) { spMag_ = mag; }

	int GetGageMaxCnt(void) { return gageMaxCnt_; }

	virtual bool IsStartFin() = 0;
protected:
	/// <summary> UI座標 </summary>
	Math::Vector2 pos_;

	/// <summary> UIサイズ </summary>
	Math::Vector2 UISize_;

	/// <summary> 死亡フラグ </summary>
	bool isAlive_;

	/// <summary> 所持アイテム一覧 </summary>
	/// <param name="int"> アイテムID </param>
	/// <param name="bool"> 先頭かどうか </param>
	std::vector<std::pair<int, bool>> order_;

	/// <summary> コントローラー情報 </summary>
	std::unique_ptr<Controller> controller_;

	/// <summary> アイテムID </summary>
	std::vector<IdData> id_;

	/// <summary> 使用されたアイテム </summary>
	ItemName useItem_;

	/// <summary> マスク情報 </summary>
	int mask_;

	/// <summary> スタミナ値 </summary>
	float stamina_;

	/// <summary> スピード倍率 </summary>
	float spMag_;

	// ゲージメモリ数最大値
	int gageMaxCnt_;

	float count_;

	std::function<void(float, const std::shared_ptr<ObjManager>)> updateFunc_;
	std::function<void(double)> drawFunc_;
};

