#pragma once
#include "GimicObj.h"

class TmxObj;
class Collider;
class ObjManager;

class Door :
	public GimicObj
{
public:
	Door(int myIdx, ObjManager& objMng, std::weak_ptr< GmkEvent>& gmkEvent,std::shared_ptr<TmxObj>& tmxObj, const Math::Vector2& pos);
	~Door();
private:
	ObjID GetObjID() final { return ObjID::DoorL; }
	void Draw(const Math::Vector2& offset, DrawMng& drawMng) final;
	void Update(float dleta) final;

	/// <summary> 閉じている時の処理 </summary>
	/// <param name="delta"></param>
	void Close(float delta);

	/// <summary> 開いている時の処理 </summary>
	/// <param name="dleta"></param>
	void Open(float dleta);

	/// <summary> 開けている途中の処理 </summary>
	/// <param name="dleta">デルタタイム </param>
	void Opening(float dleta);

	/// <summary> 閉じている途中の処理 </summary>
	/// <param name="delta"> デルタタイム </param>
	void Closing(float delta);

	/// <summary> 見つかった時の処理 </summary>
	/// <param name="delte"></param>
	void Lock(float delte);

	/// <summary> 何かに当たった時の処理 </summary>
	/// <param name="other"> 当たった相手のコライダー </param>
	void Hit(Collider& other);

	// 更新処理の関数ポインタ
	void (Door::* update_)(float);


	void StartAlert(void);

	void EndAlert(void);

	// 現在の状態になってからの時間
	float stateTime_;

	// ドアの状態
	enum class State
	{
		Close,		// 閉じている
		Opening,	// 開けている途中
		Open,		// 開いている
		Closing,		// 閉じている途中
		Lock		// 
	};

	/// <summary> ドアの状態をセットする </summary>
	/// <param name="state"> セットする状態 </param>
	void SetState(Door::State state);

	

	// 自身のコリジョンを示す番号
	int myIdx_;

	// 自身の状態
	State state_;

	// tmxObj
	std::shared_ptr<TmxObj> tmxObj_;

	// lock状態への以降フラグ
	bool lockFlag_;

	ObjManager& objMng_;
};

