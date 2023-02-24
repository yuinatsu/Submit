#pragma once
#include <concepts>
#include <unordered_map>
#include <memory>
#include <type_traits>
#include "../SoundPross.h"
#include "../../Input/Controller.h"
#include "MenuUI.h"
#include "Cursor.h"


class Controller;
class Cursor;

// メニューUIをコントロールするクラス(テンプレートでenum classを受け取る想定コンセプト使いたかった)
template<class MenuT>
class MenuUICtrl
{
public:
	
	/// <summary> コンストラクタ </summary>
	/// <param name="controller"> コントローラー </param>
	/// <param name="eMax"> メニューの最大値 </param>
	/// <param name="curthickness"> カーソル用の画像 </param>
	/// <param name="start"> 開始メニューの位置 </param>
	MenuUICtrl(Controller& controller, const int eMax, float curthickness, MenuT start) :
		controller_{ controller }, eMax_{ static_cast<std::underlying_type<MenuT>::type>(eMax) }, cursor_{ curthickness }, nowMenu_{start}
	{

	}

	~MenuUICtrl()
	{

	}

	/// <summary> アップデート </summary>
	/// <param name="delta"> デルタタイム </param>
	void Update(float delta);

	/// <summary> 描画 </summary>
	/// <param name=""></param>
	void Draw(void);

	/// <summary> メニューの追加 </summary>
	/// <param name="menu"> メニュー </param>
	/// <param name="ui"> ユニークポインタでMenuUIの右辺値参照 </param>
	void AddMenu(MenuT menu, std::unique_ptr<MenuUI>&& ui);

	/// <summary> 現在カーソルが合わさっているメニューを取得 </summary>
	/// <param name=""></param>
	/// <returns> 現在のメニュー </returns>
	const MenuT GetNowMenu(void)const;


	void SetFlag(bool flag)
	{
		flag_ = flag;
	}
	/// <summary> 指定のメニューのＵＩを取得する </summary>
	/// <param name="menu">  </param>
	/// <returns></returns>
	MenuUI& GetUI(MenuT menu);

	/// <summary> リリース処理 </summary>
	/// <param name=""></param>
	void Relese(void)
	{
		cursor_.Relese();
		for (auto& ui : menuMap_)
		{
			ui.second->Relese();
		}
	}

	/// <summary> メニューのマイナス方向への移動のIDをセットす </summary>
	/// <param name="id"></param>
	void SetMinusInputID(InputID id);

	/// <summary> メニューのプラス方向への移動のIDをセットす </summary>
	/// <param name="id"></param>
	void SetPlusInputID(InputID id);

private:

	// マイナス時のInputID
	InputID minus_ = InputID::Up;

	// プラス時のInputID
	InputID plus_ = InputID::Down;

	// 現在のメニュー
	MenuT nowMenu_;

	// コントローラーの参照
	Controller& controller_;

	// テンプレートで受け取ったenum classの最大値を基底の型で持つ
	const int eMax_;


	bool flag_ = true;
	// メニューをenum clasをキーでメニュー表示用クラスで持つ
	std::unordered_map<MenuT, std::unique_ptr<MenuUI>> menuMap_;

	// メニュー用カーソル
	Cursor cursor_;
};

template<class MenuT>
inline void MenuUICtrl<MenuT>::Update(float delta)
{
	cursor_.Update(delta);

	if (controller_.Pressed(minus_)) // メニューをマイナス方向へ
	{
		SoundProcess::PlayBackSound(SoundProcess::SOUNDNAME_SE::select,SoundProcess::GetSEVolumeEntire(),false);
		nowMenu_ = static_cast<MenuT>((static_cast<std::underlying_type<MenuT>::type>(nowMenu_) + (eMax_ - 1)) % eMax_);
		cursor_.ResetBlink();
	}
	if (controller_.Pressed(plus_)) // メニューをプラス方向へ
	{
		SoundProcess::PlayBackSound(SoundProcess::SOUNDNAME_SE::select,SoundProcess::GetSEVolumeEntire(), false);
		nowMenu_ = static_cast<MenuT>((static_cast<std::underlying_type<MenuT>::type>(nowMenu_) + 1) % eMax_);
		cursor_.ResetBlink();
	}

}

template<class MenuT>
inline void MenuUICtrl<MenuT>::Draw(void)
{
	// メニューを描画
	for (auto& menu : menuMap_)
	{
		menu.second->Draw();
	}

	// 現在のメニューの上にカーソルを表示する
	if (flag_)
	{
		cursor_.Draw(menuMap_[nowMenu_]->GetLT(), menuMap_[nowMenu_]->GetRB());
	}
}

template<class MenuT>
inline void MenuUICtrl<MenuT>::AddMenu(MenuT menu, std::unique_ptr<MenuUI>&& ui)
{
	// 指定されたメニューにMenuUIを入れる
	menuMap_[menu] = std::move(ui);
}

template<class MenuT>
inline const MenuT MenuUICtrl<MenuT>::GetNowMenu(void) const
{
	return nowMenu_;
}

template<class MenuT>
inline MenuUI& MenuUICtrl<MenuT>::GetUI(MenuT menu)
{
	return *menuMap_[menu];
}

template<class MenuT>
inline void MenuUICtrl<MenuT>::SetMinusInputID(InputID id)
{
	minus_ = id;
}

template<class MenuT>
inline void MenuUICtrl<MenuT>::SetPlusInputID(InputID id)
{
	plus_ = id;
}
