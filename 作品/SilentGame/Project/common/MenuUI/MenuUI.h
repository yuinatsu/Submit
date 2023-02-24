#pragma once
#include <string>
#include "../Math.h"

// メニュー用UI
class MenuUI
{
public:

	/// <summary> メニュー用UIのコンストラクタ </summary>
	/// <param name="pos"> 座標 </param>
	/// <param name="imgName"> 画像名 </param>
	/// <param name="imgNum"> 画像のマス目 </param>
	/// <param name="exRate"> 画像の拡大率 </param>
	MenuUI(const Math::Vector2& pos,const std::string& imgName, int imgNum = 0,float exRate = 1.0f);

	MenuUI(const MenuUI& menuUI);
	~MenuUI();

	/// <summary> 描画 </summary>
	/// <param name=""></param>
	void Draw(void);

	/// <summary> 左上座標の取得 </summary>
	/// <param name=""></param>
	/// <returns> 左上座標 </returns>
	const Math::Vector2 GetLT(void);

	/// <summary> 右下座標の取得 </summary>
	/// <param name=""></param>
	/// <returns> 右下座標 </returns>
	const Math::Vector2 GetRB(void);

	/// <summary> 解放処理 </summary>
	/// <param name=""></param>
	void Relese(void);
private:

	// 画像の番号
	int imgNum_;

	// 拡大率
	float exRate_;

	// 画像名
	std::string imgName_;

	// 座標
	Math::Vector2 pos_;

	// 画像サイズ
	Math::Vector2 imgSize_;
};

