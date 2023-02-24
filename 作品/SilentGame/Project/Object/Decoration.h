#pragma once
#include <functional>
#include "Obj.h"

class TmxObj;

// マップの装飾用のオブジェクト
class Decoration :
	public Obj
{
public:

	/// <summary> 当たり判定無しアニメーションありのコンストラクタ </summary>
	/// <param name="fname"> アニメーションファイル名 </param>
	/// <param name="state"> アニメーションのステート名 </param>
	/// <param name="key"> アニメーションのキー </param>
	/// <param name="pos"> 座標 </param>
	Decoration(const std::string& fname, const std::string& state, const std::string& key, const Math::Vector2& pos);

	/// <summary> 当たり判定ありアニメーションありのコンストラクタ </summary>
	/// <param name="fname"> アニメーションのファイル名 </param>
	/// <param name="state"> アニメーションのステート名 </param>
	/// <param name="key"> アニメーションのキー </param>
	/// <param name="r"> 当たり判定のキー </param>
	/// <param name="pos"> 座標 </param>
	Decoration(const std::string& fname, const std::string& state, const std::string& key, std::shared_ptr<TmxObj>& tmxObj, float r, const Math::Vector2& pos);

	/// <summary> 当たり判定無しアニメーション無しのコンストラクタ </summary>
	/// <param name="fname"> ファイル名 </param>
	/// <param name="key"> 画像のキー </param>
	/// <param name="divSize"> 分割サイズ </param>
	/// <param name="divNum"> 分割数 </param>
	/// <param name="idx"> 画像の分割後のマス目 </param>
	/// <param name="pos"> 座標 </param>
	Decoration(const std::string& fname, const std::string& key, const Math::Vector2I& divSize, const Math::Vector2I& divNum, int idx, const Math::Vector2& pos);

	/// <summary> 当たり判定ありアニメーション無しのコンストラクタ </summary>
	/// <param name="fname"> ファイル名 </param>
	/// <param name="key"> 画像のキー </param>
	/// <param name="divSize"> 分割サイズ </param>
	/// <param name="divNum"> 分割数 </param>
	/// <param name="idx"> 画像の分割後のマス目 </param>
	/// <param name="r"> 当たり判定の半径 </param>
	/// <param name="pos"> 座標 </param>
	Decoration(const std::string& fname, const std::string& key, const Math::Vector2I& divSize, const Math::Vector2I& divNum, int idx, std::shared_ptr<TmxObj>& tmxObj, float r, const Math::Vector2& pos);

	~Decoration();
private:

	/// <summary> アニメーションの初期化処理 </summary>
	/// <param name="fname"></param>
	/// <param name="state"></param>
	/// <param name="key"></param>
	/// <returns></returns>
	bool InitAnim(const std::string& fname, const std::string& state, const std::string& key);

	/// <summary>
	/// 当たり判定の初期化
	/// </summary>
	/// <param name="tmxObj"> tmx </param>
	/// <param name="r"> 当たり判定の半径 </param>
	/// <returns></returns>
	bool InitCollision(std::shared_ptr<TmxObj>& tmxObj, float r);

	/// <summary>
	/// 画像の初期化処理
	/// </summary>
	/// <param name="fname"> ファイルネーム </param>
	/// <param name="key"> キー </param>
	/// <param name="divSize"> 分割サイズ </param>
	/// <param name="divNum"> 分割数 </param>
	/// <param name="idx"> 表示する画像の番目 </param>
	/// <returns></returns>
	bool InitImage(const std::string& fname, const std::string& key, const Math::Vector2I& divSize, const Math::Vector2I& divNum, int idx);

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="offset"> オフセット </param>
	/// <param name="drawMng"> 描画管理クラス </param>
	void Draw(const Math::Vector2& offset, DrawMng& drawMng) final;


	ObjID GetObjID() final { return ObjID::Decoration; }
	std::function<void(const Math::Vector2&, DrawMng&)> drawFunc_;
};

