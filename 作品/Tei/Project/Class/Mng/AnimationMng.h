#pragma once
#include <list>
#include <map>
#include <string>
#include <utility>
#include "../Object/characterID.h"
#include "../Animation/Anim_ID.h"
#include "../common/Vector2.h"

struct AnimData
{
	// 表示時間
	std::list<int> flameData;
	// 添え字
	std::list<int> subscript;
};

struct AnimInfo
{
	int width = 0;
	int height = 0;
	int widthCnt = 0;
	int heightCnt = 0;
	int loop = 0;
	int widthOffset = 0;
	int heigthOffset = 0;
	std::string image = "";
	std::string imgKey = "";
};

#define lpAnimMng AnimationMng::GetInstance()
using Animation = std::pair<AnimInfo,AnimData>;

class AnimationMng
{
public:
	static AnimationMng& GetInstance()
	{
		static AnimationMng s_instance_;
		return s_instance_;
	}
	/// <summary>
	/// キャラクターに対応したアニメーションデータの読み込みと登録
	/// </summary>
	/// <param name="filepath">tmxへのパス</param>
	/// <param name="id">登録するキャラのID</param>
	/// <returns>true:成功 false:失敗</returns>
	bool LoadAnimTmx(const std::string& filepath, Object_ID id);
	/// <summary>
	/// 引数で指定されたキャラのアニメーションに関するデータ
	/// 指定したキャラ、もしくは指定したアニメーションが未登録の場合中身のないAnimationが返ってくる
	/// </summary>
	/// <param name="cID">獲得したいキャラのID</param>
	/// <param name="aID">獲得したいアニメーションのID</param>
	/// <returns>アニメーション関連データ</returns>
	const Animation GetAnimationData(Object_ID cID, Char_Anim_ID aID);
	/// <summary>
	/// 引数で指定されたキャラのアニメーションのImageに関するデータ
	/// 指定したキャラ、もしくは指定したアニメーションが未登録の場合中身のないAnimInfoが返ってくる
	/// </summary>
	/// <param name="cID">獲得したいキャラのID</param>
	/// <param name="aID">獲得したいアニメーションのID</param>
	/// <returns>アニメーションの画像関連データ</returns>
	const AnimInfo GetAnimInfo(Object_ID cID, Char_Anim_ID aID);
	/// <summary>
	/// 引数で指定されたIDのInfoが存在するかどうか
	/// </summary>
	/// <param name="cID">キャラのID</param>
	/// <param name="aID">アニメーションのID</param>
	/// <returns>true:存在する false:存在しない</returns>
	bool CheckAnimInfo(Object_ID cID, Char_Anim_ID aID);
	/// <summary>
	/// 引数の情報をもとに現在表示するべき画像を返す
	/// キャラIDやアニメーションIDに対応したデータ登録されていない場合-1が返ってくる
	/// </summary>
	/// <param name="cID">キャラID</param>
	/// <param name="aID">アニメーションID</param>
	/// <param name="elapsed">経過フレーム数 ループする場合リセットされる</param>
	/// <param name="loopNum">現在アニメーション繰り返し回数</param>
	/// <returns>データがある場合表示すべき画像 ない場合-1</returns>
	const int GetAnimImag(Object_ID cID, Char_Anim_ID aID, int& elapsed, int& loopNum);
	/// <summary>
	/// 引数の情報をもとにloopが終わっているかを取得する
	/// 登録されていないキャラIDやアニメーションIDが来た場合再生は終わっているものとする
	/// </summary>
	/// <param name="cID">キャラID</param>
	/// <param name="aID">アニメーションID</param>
	/// <param name="elapsed">経過フレーム数</param>
	/// <param name="loopNum">現在繰り返し回数</param>
	/// <returns>true:終わっている又はデータの存在しないID false:ループ中</returns>
	const bool CheckAnimLoopEnd(Object_ID cID, Char_Anim_ID aID,const int& elapsed,const int& loopNum);
	/// <summary>
	/// 描画する際のoffSet獲得 データが存在しない場合{0,0}で返ってくる
	/// </summary>
	/// <param name="cID">獲得したいキャラのID</param>
	/// <param name="aID">獲得したいアニメーションのID</param>
	/// <returns>描画する際のoffset</returns>
	const Vector2 GetDrawOffSet(Object_ID cID, Char_Anim_ID aID);
	/// <summary>
	/// 指定されたIDの時の分割後画像サイズを獲得する
	/// 登録されていないデータの場合{0,0}で返ってくる
	/// </summary>
	/// <param name="cID">獲得したいキャラのID</param>
	/// <param name="aID">獲得したいアニメーションのID </param>
	/// <returns>分割後画像サイズ</returns>
	Vector2 GetDivImageSize(Object_ID cID, Char_Anim_ID aID);
private:
	AnimationMng();
	~AnimationMng() = default;
	// キャラクターの種別とアニメーション種別をキーにしたアニメーション関連データ
	std::map<Object_ID, std::map<Char_Anim_ID,Animation>> animData_;
};
