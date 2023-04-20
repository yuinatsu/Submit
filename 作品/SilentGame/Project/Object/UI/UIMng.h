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
	// コンストラクタ 
	UIMng();
	~UIMng();

	// 更新 
	bool Update(float delta, const std::shared_ptr<ObjManager> objMng);

	// 描画 
	void Draw(const double& delta);

	// itemUIのshered_ptr取得 
	std::shared_ptr<UiBase> GetItemUI() { return itemUI_; }

	// staminaUIのshered_ptr取得 
	std::shared_ptr<UiBase> GetStaminaUI() { return staminaUI_; }

	// 開始時演出終了
	bool FinStart();
private:
	// アイテム取得と使用アイテムの更新 
	bool UpdateGetUse(float delta, const std::shared_ptr<ObjManager> objMng);

	// スピード倍率更新 
	bool UpdateSpMag(const std::shared_ptr<ObjManager> objMng);

	// アイテムUI 
	std::shared_ptr<UiBase> itemUI_;

	// スタミナUI 
	std::shared_ptr<UiBase> staminaUI_;

	// スタミナ動作フラグ 
	bool stanimaF_;

	// エフェクトフラグ
	bool effectF_;
};

