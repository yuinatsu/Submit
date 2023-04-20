#pragma once
#include <memory>
#include <vector>
#include <string>
#include <functional>
#include "../Item/ItemName.h"
#include "../../common/Math.h"
#include "../ObjManager.h"

// 最大アイテム所持数 
constexpr int haveItemMax = 6;

// pair<ID, アイテム名> 
using IdData = std::pair<int, std::string>;

// UIの種類 
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
	// コンストラクタ 
	UiBase(const Math::Vector2& pos, const Math::Vector2& UISize);
	virtual ~UiBase() = default;

	// 更新 
	virtual void Update(const double& delta, const std::shared_ptr<ObjManager> objMng) = 0;

	virtual void UpdateStart(const double& delta, const std::shared_ptr<ObjManager> objMng) = 0;

	virtual void UpdateGame(const double& delta, const std::shared_ptr<ObjManager> objMng) = 0;

	// アイテム情報更新 
	void UpdateItem(const double& delta);

	// UI部分描画 
	virtual void DrawUI(const double& delta) = 0;

	virtual void DrawStart(const double& delta) = 0;

	virtual void DrawGame(const double& delta) = 0;

	// アイテムIDセット 
	bool SetItemID(IdData id);

	// アイテムID取得 
    std::vector<IdData> GetItemID() { return id_; };

	// 使用されたアイテムを取得 
	ItemName GetUseItem();

	// 生存フラグ 
	bool isAlive(void) { return isAlive_; };

	// ID取得 
	virtual UIID GetUIID(void) = 0;

	// スタミナ値取得 
	float GetStamina(void) { return stamina_; }

	// スピード倍率セット 
	void SetSpMag(float mag) { spMag_ = mag; }

	// ゲージ最大値取得
	int GetGageMaxCnt(void) { return gageMaxCnt_; }

	virtual bool IsStartFin() = 0;
protected:
	// UI座標 
	Math::Vector2 pos_;

	// UIサイズ 
	Math::Vector2 UISize_;

	// 死亡フラグ 
	bool isAlive_;

	// 所持アイテム一覧 
	std::vector<std::pair<int, bool>> order_;

	// コントローラー情報 
	std::unique_ptr<Controller> controller_;

	// アイテムID 
	std::vector<IdData> id_;

	// 使用されたアイテム 
	ItemName useItem_;

	// マスク情報 
	int mask_;

	// スタミナ値 
	float stamina_;

	// スピード倍率 
	float spMag_;

	// ゲージメモリ数最大値
	int gageMaxCnt_;

	// ゲーム内時間カウンタ
	float count_;

	// 切り替え用
	std::function<void(float, const std::shared_ptr<ObjManager>)> updateFunc_;
	std::function<void(double)> drawFunc_;
};

