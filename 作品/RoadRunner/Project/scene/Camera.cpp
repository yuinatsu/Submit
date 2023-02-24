#include <algorithm>
#include "Camera.h"
#include "Obj/Obj.h"
#include "MapCtl.h"
#include "SceneMng.h"
#include "../_debug/_DebugDispOut.h"


Camera::Camera()
{
	// pos_はゼロクリの時点で初期化されてるので書く必要ない
}


Camera::~Camera()
{
}


bool Camera::InitArea(void)
{
	Vector2 tmpPos = (lpMapCtl.viewArea_ / 2) * lpMapCtl.chipSize_;						// viewArea_の半分の大きさ×chipSize_

	antiMoveRect_.left = tmpPos.x;					// worldArea_の中心(左端からtmpPos分右にずれた所)の左点
	antiMoveRect_.top = tmpPos.y;					// カメラの上下の動きをさせない
	antiMoveRect_.right = (lpMapCtl.worldArea_.x - ((lpMapCtl.viewArea_.x / 2) + lpMapCtl.viewArea_.x % 2)) * lpMapCtl.chipSize_.x;				// worldArea_の中心(worldArea_の右端からtmpPos分左にずれた所)の右点
	antiMoveRect_.bottom = antiMoveRect_.top;				// topと同じ値を入れる

	defPos_ = { antiMoveRect_.left,antiMoveRect_.top };					// デフォルト座標を設定

	return false;
}


bool Camera::SetTarget(std::weak_ptr<Obj> target)
{
	target_ = target;
	if (target_.expired())				// target_の寿命が尽きているかを調べる			expired ... 寿命が尽きているかを調べる（死んでる：true, 生きてる：false）
	{
		// カメラが死んでいる場合
		return false;
	}
	// カメラが生きている場合

	InitArea();

	pos_.x = target.lock()->GetPos().x;										//	プレイヤーのX座標を取得					weak_ptrは(*target)->などでアクセスはできない
	pos_.x = std::clamp(pos_.x, static_cast<int>(antiMoveRect_.left), static_cast<int>(antiMoveRect_.right));						// X座標のマス目の範囲チェック(プレイヤーの座標を直接入れたらworldArea_外を表示してしまう可能性があるため)
	pos_.y = antiMoveRect_.top;

	return true;
}


void Camera::Update(void)
{
	if (!target_.expired())
	{
		pos_.x = target_.lock()->GetPos().x;								//	プレイヤーのX座標を取得
		pos_.x = std::clamp(pos_.x, static_cast<int>(antiMoveRect_.left), static_cast<int>(antiMoveRect_.right));						// X座標のマス目の範囲チェック
	}
}


void Camera::Draw(void)
{
	auto offset = lpSceneMng.drawOffset_;							// カメラの中心位置をオフセット分ずらす
	_dbgDrawBox(pos_.x + offset.x, pos_.y + offset.y, pos_.x + lpMapCtl.chipSize_.x + offset.x, pos_.y + lpMapCtl.chipSize_.y + offset.y, 0x00ff00, true);						// (カメラのX座標, カメラのY座標, カメラのX座標 + チップサイズ, カメラのY座標 + チップサイズ, 緑色（デバッグ用）, true) ... 四頂点すべてにオフセットを足す
}


void Camera::SetPos(const Vector2& pos)
{
	pos_.x = std::clamp(pos.x, static_cast<int>(antiMoveRect_.left), static_cast<int>(antiMoveRect_.right));
	pos_.y = std::clamp(pos.y, static_cast<int>(antiMoveRect_.top), static_cast<int>(antiMoveRect_.bottom));
}


const Vector2& Camera::GetPos(void)
{
	return pos_;
}


const Vector2 Camera::GetDrawOffset(void)
{
	return (defPos_ - pos_);
}