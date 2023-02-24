#pragma once
class ResultData
{
public:
	ResultData();
	~ResultData();

	/// <summary> 倒した数を引数分加算する </summary>
	/// <param name="val"> 加算する倒した数 </param>
	void AddKillCount(int val);

	/// <summary> 見つかった数を+1する </summary>
	/// <param name=""></param>
	void CountUpAlertCount(void);

	/// <summary> クリアまでの時間を更新する </summary>
	/// <param name="delta"></param>
	void UpdateClearTime(float delta);

	/// <summary> 倒した数を取得 </summary>
	/// <param name=""></param>
	/// <returns> 倒した数 </returns>
	const int GetKillCount(void)const { return killCount_; }

	/// <summary> リザルトまでの経過時間を取得 </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const float GetClearTime(void) const { return clearTime_; }

	/// <summary> 見つかった数を取得 </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const int GetAlertCount(void) const { return alertCount_; }

	/// <summary> クリアかどうかをセットする </summary>
	/// <param name="flag"></param>
	void SetClear(bool flag);

	const bool IsClear(void) const { return isClear_; }

	void CountUpItemUseCount(void);

	const int GetItemUseCount(void) const { return itemUseCount_; }
private:

	// クリアかゲームオーバーか
	bool isClear_;

	// 倒した数
	int killCount_;

	// リザルトまでの時間
	float clearTime_;

	// アラート状態になった回数
	int alertCount_;

	// アイテム使用回数
	int itemUseCount_;
};

