#pragma once
#include <map>
#include <vector>
#include <string>
#include "../common/AnimationData.h"
#include "Component.h"
#include "../common/Math.h"

using AnimVector = std::vector<std::pair<int, int>>;

class DrawMng;


class Animation :
	public Component
{
public:

	/// <summary> データファイルとキーと初期ステート指定のコンストラクタ </summary>
	/// <param name="fname"> アニメーションデータのファイル名 </param>
	/// <param name="key"> キー </param>
	/// <param name="state"> 初期ステート </param>
	/// <param name="name"> コンポーネント名 </param>
	/// <param name="owner"> 持ち主 </param>
	Animation(const std::string& fname, const std::string& key, const std::string& state, Obj& owner);
	
	~Animation();

	void Update(float delta) override;											// 更新
	bool Draw(DrawMng& drawMng,Math::Vector2 pos, bool flag);		// 描画 Draw(座標, サイズ, 方向)
	bool Draw(DrawMng& drawMng, Math::Vector2 pos, bool flag, const std::string& key);		// 描画 Draw(座標, サイズ, 方向)
	bool Draw(DrawMng& drawMng, Math::Vector2 pos, Math::Vector2I size, std::string state);		// 描画 Draw(座標, サイズ, 方向)

	/// <summary> 深度指定の描画 </summary>
	/// <param name="drawMng"> 描画管理用クラス </param>
	/// <param name="pos"> 描画する座標 </param>
	/// <param name="flag"> idel </param>
	/// <returns></returns>
	bool Draw(DrawMng& drawMng, Math::Vector2 pos, float depth = 0.0f, bool flag = false);

	std::string GetKey(void);									// キー情報取得
	std::string GetState(void);									// ステータス取得
	

	/// <summary> 状態をセット </summary>
	/// <param name="state"> ステート </param>
	void SetState(const std::string& state);

	/// <summary> 状態をセット（状態が同じでも通る） </summary>
	/// <param name="state"></param>
	void SetStateSame(const std::string& state);

	void SetKey(const std::string& key);

	/// <summary> 再生終了しているか？ </summary>
	/// <param name=""></param>
	/// <returns></returns>
	bool IsEnd(void) const;

	/// <summary>
	/// 表示する画像の何番目を示すかを返す
	/// </summary>
	/// <param name=""></param>
	/// <returns> 何番目か </returns>
	const int GetNowIdx(void)const;
private:

	ComponentID GetID() const override;

	// アニメーションデータのキー
	std::string key_;

	// ステート
	std::string state_;

	// ループ数
	int loop_;

	// 再生時間
	float playStepTime_;

	// 現在の再生位置
	std::list<std::pair<int, float>>::const_iterator nowItr_;

};

