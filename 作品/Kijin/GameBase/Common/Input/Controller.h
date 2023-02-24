#pragma once
#include <map>
#include <array>
#include <string>
#include "InputID.h"
#include "../Vector2.h"
#include "../../Application.h"

// トリガー情報
enum class Trg
{
	Now,				// 現在
	Old,				// 一つ前
	Max
};

// 入力装置の種類
enum class CntType
{
	Key,				// キーボード
	Pad,				// パッド
	Max
};

//using TrgBool = std::array<bool, static_cast<size_t>(Trg::Max)>;	// 入力状況		std::array<入力真偽, トリガー情報>
//using CntData = std::map<InputID, TrgBool>;							// 入力情報		std::map<キー情報, TrgBool>
using InputData = std::array<std::pair<bool, bool>, static_cast<size_t>(InputID::Max) + 1>;

class Controller
{
public:
	bool operator()(float delta)
	{
		if (this != nullptr)
		{
			Update(delta);
		}
		return this != nullptr;
	}
	Controller();
	virtual ~Controller();

	/// <summary> 初期化 </summary>
	/// <returns> 成功時true失敗時false </returns>
	virtual bool Init(void) = 0;

	/// <summary> 更新 </summary>
	/// <param name="delta"> デルタタイム </param>
	virtual void Update(float delta) = 0;

	/// <summary> コントローラーの種類の取得 </summary>
	/// <returns> コントローラーの種類 </returns>
	virtual CntType GetCntType(void) = 0;

	/// <summary> 今左クリックしているか </summary>
	/// <param name="id">キーの種類</param>
	/// <returns>成功時にtrue失敗時にfalse</returns>
	bool MousePress(InputID id);

	/// <summary> 今押しているか </summary>
	/// <param name="id"> キーの種類 </param>
	/// <returns> 成功時true失敗時false </returns>
	bool Press(InputID id);

	/// <summary> 押した瞬間 </summary>
	/// <param name="id"> キーの種類 </param>
	/// <returns> 成功時true失敗時false </returns>
	bool Pressed(InputID id);

	/// <summary> 離した瞬間 </summary>
	/// <param name="id"> キーの種類 </param>
	/// <returns> 成功時true失敗時false </returns>
	bool Released(InputID id);

	/// <summary> 今押されていないとき </summary>
	/// <param name="id"> キーの種類 </param>
	/// <returns> 成功時true失敗時false </returns>
	bool NotPress(InputID id);

	/// <summary> 今何も押されていないか </summary>
	/// <returns> 成功時true失敗時false </returns>
	bool IsNeutral();

	/// <summary> 何か押されたか </summary>
	/// <returns> 成功時true失敗時false </returns>
	bool IsAnyPress();

	/// <summary>
	/// カーソル位置をセット(デフォルト引数は中心)
	/// </summary>
	/// <param name="pos"> カーソルの位置 </param>
	virtual void SetCursorPos(const Vector2& pos = lpApp.screenSize_<float> / 2.0f) = 0;

	/// <summary>
	/// カーソル位置の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns> カーソルの座標 </returns>
	const Vector2& GetCursorPos(void) const&
	{
		return cursorPos_;
	}


	/// <summary>
	/// カーソルのスピードをセット
	/// </summary>
	/// <param name="speed"> スピード </param>
	void SetCursorSpeed(float speed)
	{
		cursorSpeed_ = speed;
	}

	const float GetCursorSpeed(void) const
	{
		return cursorSpeed_;
	}

	const Vector2& GetInputVec(void) const&
	{
		return inputVec_;
	}

	/// <summary> 長押し </summary>
	/// <param name="id"> キーの種類 </param>
	/// <param name="limit"> 長押しの最大時間 </param>
	/// <param name="delta"> デルタタイム </param>
	/// <returns> 成功時true失敗時false </returns>
	std::string LongPress(InputID id, double limit, double delta);
	const InputData& Get(void);
private:
protected:
	/// <summary> 入力情報 </summary>
	//CntData cntData_;
	InputData data_;

	/// <summary> キーのリスト情報 </summary>
	//std::map<InputID, unsigned int> keyList_ = {};

	// カーソルの座標
	Vector2 cursorPos_;

	// カーソルのスピード(padしか使わん)
	float cursorSpeed_;

	// 移動用の入力された方向
	Vector2 inputVec_;

	/// <summary> LongPress関係 </summary>
	bool isPress_;
	bool isFinish_;
	float PressCount_;
};

