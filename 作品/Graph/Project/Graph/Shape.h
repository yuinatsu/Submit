#pragma once
#include <memory>
#include <vector>
#include "Vector2.h"

// 図形種類
enum class ShapeType
{
	Square,
	Circle,
	Triangle,
	Star,
	Tree,
	Micky,
	Max
};

// 縦移動方向
enum class MoveUD
{
	Up,
	Down,
	Max
};

// 横移動方向
enum class MoveLR
{
	Left,
	Right,
	Max
};

// 壁の位置
enum class WallDir
{
	Up,
	Down,
	Left,
	Right,
	Max
};

class Shape;
using ShapeVec = std::vector<std::unique_ptr<Shape>>;

class Shape
{
public:
	Shape(Vector2Flt& pos, Vector2Flt& size);                  // コンストラクタ(座標, サイズ)
	~Shape();
	virtual void Update(float delta, ShapeVec& shapeVec) = 0;		// 更新(deltaTime, ShapeVec)
	virtual void Draw(void) = 0;									// 描画
	virtual void Draw(float num) = 0;								// 描画(拡大サイズ)
	virtual void Draw(float num, Vector2Flt offset) {};				// 描画(拡大サイズ, 中心座標)
	virtual ShapeType GetType() = 0;								// 図形種類を取得
	virtual Vector2Flt GetPos() = 0;								// 座標取得
	virtual Vector2Flt GetSize() = 0;								// サイズ取得
	virtual int GetColor() = 0;										// 図形色取得
private:
protected:
	void UpdateMoveDir(float up, float down, float left, float right);												// 移動方向チェック
	// 当たり判定
	bool IsHitCircleAndSquare(Vector2Flt cPos, Vector2Flt cSize, Vector2Flt sPos, Vector2Flt sSize);				// 円*矩形
	bool IsHitCircleAndTriangle(Vector2Flt cPos, Vector2Flt cSize, Vector2Flt tPos, Vector2Flt tSize);				// 円*三角
	bool IsHitCircleAndTree(Vector2Flt cPos, Vector2Flt cSize, Vector2Flt tPos, Vector2Flt tSize);					// 円*ツリー
	bool IsHitCircleAndMicky(Vector2Flt cPos, Vector2Flt cSize, Vector2Flt mPos, Vector2Flt mSize);					// 円*ミッキー
	bool IsHitCircleAndStar(Vector2Flt cPos, Vector2Flt cSize, Vector2Flt sPos, Vector2Flt sSize);					// 円*星
	bool IsHitSquareAndTriangle(Vector2Flt sPos, Vector2Flt sSize, Vector2Flt tPos, Vector2Flt tSize);				// 矩形*三角
	bool IsHitSquareAndTree(Vector2Flt sPos, Vector2Flt sSize, Vector2Flt tPos, Vector2Flt tSize);					// 矩形*ツリー
	bool IsHitSquareAndMicky(Vector2Flt sPos, Vector2Flt sSize, Vector2Flt mPos, Vector2Flt mSize);					// 矩形*ミッキー
	bool IsHitSquareAndStar(Vector2Flt sPos, Vector2Flt sSize, Vector2Flt stPos, Vector2Flt stSize);				// 矩形*星
	bool IsHitTriangleAndTree(Vector2Flt tPos, Vector2Flt tSize, Vector2Flt treePos, Vector2Flt treeSize);			// 三角*ツリー
	bool IsHitTriangleAndMicky(Vector2Flt tPos, Vector2Flt tSize, Vector2Flt mPos, Vector2Flt mSize);				// 三角*ミッキー
	bool IsHitTriangleAndStar(Vector2Flt tPos, Vector2Flt tSize, Vector2Flt sPos, Vector2Flt sSize);				// 三角*星

	Vector2Flt pos_;		// 座標
	Vector2Flt size_;		// サイズ

	Vector2 scrSize_;		// スクリーンサイズ

	MoveLR moveLR_;			// 左右移動方向
	MoveUD moveUD_;			// 上下移動方向

	WallDir hitWall_;		// 壁判定

	int color_;				// 色
	int speed_;				// 移動量
};
