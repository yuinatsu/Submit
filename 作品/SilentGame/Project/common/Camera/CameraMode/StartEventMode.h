#pragma once
#include <functional>
#include <vector>
#include "../../Math.h"
#include "CameraModeBase.h"

class Obj;
class Controller;

class StartEventMode :
    public CameraModeBase
{
public:
	StartEventMode(Controller& controller,Camera& camera, const std::weak_ptr<Obj>& start, const std::weak_ptr<Obj>& end);
	~StartEventMode();
private:
	void Update(Camera& camera, float delta) final;
	void Draw(Camera& camera) final;

	/// <summary> マップを見せるための移動 </summary>
	/// <param name="camera"></param>
	void MoveMapUpdate(Camera& camera,float dleta);

	/// <summary> ターゲットまで移動 </summary>
	/// <param name="camera"></param>
	void MoveTargetUpdate(Camera& camera, float dleta);
	
	/// <summary> ターゲットを表示して待機する </summary>
	/// <param name="camera"></param>
	void WaitTargetUpdate(Camera& camera, float dleta);

	/// <summary> 元の位置に戻る </summary>
	/// <param name="camera"></param>
	void BackUpdate(Camera& camera, float dleta);

	void Skip(Camera& camera);

	void Loaded(void) final;

	// 開始位置
	std::weak_ptr<Obj> start_;

	// 片道の終点
	std::weak_ptr<Obj> end_;

	// 開始時に見せる座標
	std::vector<Math::Vector2> positions_;

	// 上記のイテレーター
	std::vector<Math::Vector2>::const_iterator posItr_;

	// 移動開始座標
	Math::Vector2 startPos_;

	// 移動終了座標
	Math::Vector2 endPos_;

	// スキップ時の倍速する値
	float skipVal_;

	// 
	std::function<void(Camera&,float)> updateFunc_;
	
	// 上下の枠の値
	float frameVal_;

	// 入力情報
	Controller& controller_;

	// スキップできることを表示する座標
	Math::Vector2 skipPos_;

	// 入力情報の一文字分の大きさ
	int inputOffset_;

	// 入力情報の画像のキー
	std::string skipInput_;

	int inputCode_;

	float skipTime_;

	enum class Mode
	{
		MoveMap,
		Target,
		Wait,
		Back
	};

};

