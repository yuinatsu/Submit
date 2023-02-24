#pragma once
#include <memory>
#include "../common/Math.h"

enum class DescriptionID
{
	LandMine,
	Decoy,
	Alcohol,
	Clock,
};

template<class T>
class MenuUICtrl;

class Controller;

// アイテムの説明の表示するクラス
class ItemDescriptionWnd
{
public:
	ItemDescriptionWnd(Controller& controller,const Math::Vector2& lt, const Math::Vector2& rb);
	bool Update(float delta);
	void Draw(void);
	// アイテムを選択しているかを受け取る
	bool GetFlag(void);
private:

	/// <summary> 何も選択してない時のアップデート </summary>
	/// <param name="delta"></param>
	/// <returns></returns>
	bool UpdateNon(float delta);

	/// <summary> 説明が表示中の時アップデート </summary>
	/// <param name="delta"></param>
	/// <returns></returns>
	bool UpdateOpen(float delta);

	/// <summary> 説明を閉じた時のアップデート </summary>
	/// <param name="delta"></param>
	/// <returns></returns>
	bool UpdateClose(float delta);

	/// <summary> 何も選択していないときの描画 </summary>
	/// <param name=""></param>
	void DrawNon(void);

	/// <summary> 説明を表示中の描画 </summary>
	/// <param name=""></param>
	void DrawOpen(void);

	/// <summary> 説明を閉じている時の描画 </summary>
	/// <param name=""></param>
	void DrawClose(void);

	/// <summary> 表示を開始する </summary>
	/// <param name="imgName"></param>
	void Start(const std::string& imgName);

	// アップデートの関数ポインタ
	bool (ItemDescriptionWnd::* update_)(float);

	// 描画の関数ポインタ
	void (ItemDescriptionWnd::* draw_)(void);

	// 左上
	Math::Vector2 lt_;

	// 右下
	Math::Vector2 rb_;

	// メニューを管理するクラス
	std::unique_ptr<MenuUICtrl<DescriptionID>> menuUI_;

	// 入力関係
	Controller& controller_;

	// 説明画像のサイズ
	Math::Vector2 imgSizeHalf_;

	// 画像名
	std::string imgName_;

	// 経過時間
	float time_;

	// 決定しているかを調べる
	bool flag_;
};

