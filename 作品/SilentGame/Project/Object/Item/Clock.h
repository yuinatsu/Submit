#pragma once
#include <functional>
#include "../Obj.h"

class ObjManager;

class Clock :
    public Obj
{
public:
	Clock(const Math::Vector2& pos, ObjManager& objMng);
	~Clock();

	// アラーム音がなっていたら止める
	void StopAlarm(void) const;

	// アラーム音がなるはずなら再度鳴らす
	void RestartAlarm(void) const;
private:
	bool Init(void);
	void Update(float delta) override;
	void Draw(const Math::Vector2& offset, DrawMng& drawMng)override;
	ObjID GetObjID(void) override { return ObjID::Clock; };
	
	/// <summary> 音がなるまでの処理 </summary>
	/// <param name=""></param>
	void NonUpdate(void);

	/// <summary> 音がなってる時の処理 </summary>
	/// <param name=""></param>
	void AlarmUpdate(void);

	// 経過時間
	float stepTime_;

	// 更新処理
	std::function<void(void)> updateFunc_;
	ObjManager& objMng_;
	// 表示時にずらすため
	Math::Vector2 randOffset_;

	// アラームがなっているか
	bool isAlarm_;
};

