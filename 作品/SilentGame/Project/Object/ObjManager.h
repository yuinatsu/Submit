#pragma once
#include <memory>
#include <list>
#include <vector>
#include <fstream>
//#include "../Object/Obj.h"
#include <map>
#include "../common/Math.h"
#include "Effect/EffectID.h"
#include "Item/ItemName.h"
#include "../common/Random.h"
#include "../common/Structure.h"

 	class Obj;
	using BoxItem = std::map<ItemName, bool>;
class TmxObj;
class GmkEvent;
class UiBase;
class BehaviorTreeLoader;
class DrawMng;
class ResultData;
class Footprints;
enum class ObjID;

class ObjManager
{
public:
	ObjManager(
		const std::string& objDataName,
		std::shared_ptr< ResultData>& resultData, 
		UiBase& itemUi,
		std::shared_ptr<GmkEvent>& gmkEvent,
		std::shared_ptr<TmxObj>& tmxObj,
		const Math::Vector2& scrSize
	);

	~ObjManager();

	bool Update(float delta);
	void UpdateEffect(float delta);
	// チュートリアル用のアップデート
	bool UpdateTutorial1(float delta, bool moveFlag, bool hitFlag);
	bool UpdateTutorial2(float delta, bool moveFlag, bool hitFlag, bool enemyDrawFlag);
	bool UpdateTutorial3(float delta, bool moveFlag, bool hitFlag,bool gmcflag);
	// ゲームクリアorゲームオーバー
	bool UpdateGameEnd(float delta);
	void Draw(const Math::Vector2& offset, DrawMng& drawMng);
	void EffectDraw(const Math::Vector2& offset, DrawMng& drawMng);		// Effectのみ描画したい時に使用
	void EffectDraw(DrawMng& drawMng);									// Effectのみ描画(オフセット無視)
	// チュートリアルごとに関数を作るけどまとめることができるのならまとめる
	void DrawTutorial1(const Math::Vector2& offset, DrawMng& drawMng);
	void DrawTutorial2(const Math::Vector2& offset, DrawMng& drawMng, bool enemyFlag);
	void DrawTutorial3(const Math::Vector2& offset, DrawMng& drawMng);
	void DrawGameEnd(const Math::Vector2& offset, DrawMng& drawMng);

	/// <summary> 開始演出時の更新処理 </summary>
	/// <param name="delta"> デルタタイム </param>
	void UpdateStart(float delta);

	/// <summary> 座標に一番近い敵を探す </summary>
	/// <param name="pos"> 座標 </param>
	/// <returns> 一番近い敵 </returns>
	std::weak_ptr<Obj> FindEnemy(const Math::Vector2& pos);

	void SortEnemyList(const Math::Vector2& pos);

	/// <summary> 指定された座標に近い順にソートされた敵のリストを返す </summary>
	/// <param name="pos"> ソートする基準の座標 </param>
	/// <returns> 敵のリスト </returns>
	std::list<std::shared_ptr<Obj>>& GetSortedEnemyList(const Math::Vector2& pos);

	/// <summary> プレイヤーを取得 </summary>
	/// <param name=""></param>
	/// <returns> プレイヤー </returns>
	std::weak_ptr<Obj> GetPlayer(void);


	/// <summary> ターゲットを取得 </summary>
	/// <param name=""></param>
	/// <returns> ターゲット </returns>
	std::weak_ptr<Obj> GetTarget(void);

	std::weak_ptr<GmkEvent> GetGmkEvent(void);
	
	// どのアイテムが入っている箱を開けたか
	BoxItem& GetBoxItem(void);

	// 使用されたアイテムのセット
	void SetUseItem(ItemName item) { useItem_ = item; };

	// アイテム使用時
	void UseItem(ItemName item);

	// 特定のアイテムを所持しているか
	bool HaveSpecItem(ItemName item);

	/// <summary> エフェクトを追加 </summary>
	/// <param name="effectName"> エフェクトの名前 </param>
	/// <param name="pos"> 発生座標 </param>
	void AddEffect(EffectName effectName, const Math::Vector2& pos);

	/// <summary> エフェクトを追加 </summary>
	/// <param name="effectName"></param>
	/// <param name="pos"></param>
	void AddEffect(EffectName_x120 effectName, const Math::Vector2& pos);

	void AddEffect(EffectNameWhole effectName, const Math::Vector2& pos);

	bool GetPlayerEntrance(void) { return pEnt_; }

	// アラートが起動しているかどうか
	bool GetAlertFlag(void);

	bool IsClear(void) { return isClear_; }

	void SetHaveItem(std::vector<std::pair<int, bool>> item) { item_ = item; }

	const std::list<std::unique_ptr<Obj>>& GetItemList(void) const { return itemList_; }
private:

	/// <summary> オブジェクト同士の当たり判定を取る </summary>
	/// <param name=""></param>
	void CheckObjHit(void);
	void CheckObjHitTutorial1(bool hitflag);
	void CheckObjHitTutorial2(bool hitflag);
	void CheckObjHitTutorial3(bool hitflag, bool gmcFlag);
	bool Load(const std::string& xmlName, UiBase& itemUi, std::shared_ptr<GmkEvent>& gmkEvent, std::shared_ptr<TmxObj>& tmxObj);


	// ヘッダー
	struct Header
	{
		char s[4];				// シグネチャ
		unsigned int ver;		// バージョン
		unsigned int sum;		// チェックSam
	};

	// データ部用のヘッダー
	struct DateHeader
	{
		ObjID id;		// オブジェクトのID
		int num;		// 個数
	};

	// オブジェクトの基本データ
	struct ObjData
	{
		Math::Vector2 pos_;		// 座標
		Math::Vector2 vec_;		// 向き
		std::string imgName_;	// 画像の名前
	};

	/// <summary> プレイヤー生成処理 </summary>
	/// <param name="tmxObj"> tmxObj </param>
	/// <param name="obj"> オブジェクトの基本データ </param>
	/// <param name="f"> ファイルストリーム </param>
	void CreatePlayer(std::shared_ptr<TmxObj>& tmxObj, ObjData& obj, std::ifstream& f);

	/// <summary> 敵の生成処理 </summary>
	/// <param name="bloder"> ビヘイビアツリーのロード用クラス </param>
	/// <param name="tmxObj"> tmxObj </param>
	/// <param name="obj"> オブジェクトの基本データ </param>
	/// <param name="f"> ファイルストリーム </param>
	void CreateEnemy(BehaviorTreeLoader& bloder, std::shared_ptr<TmxObj>& tmxObj, ObjData& obj, std::ifstream& f);

	/// <summary> クリア目標の生成処理 </summary>
	/// <param name="bloder"> ビヘイビアツリーのロード用クラス </param>
	/// <param name="tmxObj"> tmxObj </param>
	/// <param name="obj"> オブジェクトの基本データ </param>
	/// <param name="f"> ファイルストリーム </param>
	void CreateTarget(BehaviorTreeLoader& bloder, std::shared_ptr<TmxObj>& tmxObj, ObjData& obj, std::ifstream& f);


	void CreateDecoration(std::shared_ptr<TmxObj>& tmxObj,ObjData& obj, std::ifstream& f);

	void CreateDoor(std::shared_ptr<TmxObj>& tmx);

	std::shared_ptr< ResultData> resultData_;

	// プレイヤー
	std::shared_ptr<Obj> player_;

	// アイテム
	std::vector<std::pair<int, bool>> item_;

	// 敵リスト
	std::list<std::shared_ptr<Obj>> enemyList_;

	std::shared_ptr<Obj> target_;

	// ギミックリスト
	std::list<std::unique_ptr<Obj>> gimicList_;

	// アイテムリスト
	std::list<std::unique_ptr<Obj>> itemList_;

	// エフェクトリスト
	std::list<std::unique_ptr<Obj>> effectList_;

	// 現在使用されたアイテム
	ItemName useItem_;

	// ギミックイベントクラス
	std::weak_ptr<GmkEvent> gmkEvent_;

	// 再生待ちのエフェクト
	EffectName effect_;

	Random random_;

	// チュートリアルで表示するかどうか
	bool pflag_;
	bool eflag_;
	BoxItem boxItem_;
	bool alertFlag_;

	// プレイヤーが画面に表れているかどうか
	bool pEnt_;

	// クリア判定
	bool isClear_;

	std::list<std::unique_ptr<Footprints>> footprintsList_;

	std::list<std::unique_ptr<Obj>> doorList_;
};