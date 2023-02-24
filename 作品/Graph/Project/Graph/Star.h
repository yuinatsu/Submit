#pragma once
#include "Shape.h"

class Star :
    public Shape
{
public:
    Star(Vector2Flt&& pos, Vector2Flt&& size);                      // コンストラクタ(座標, サイズ)
    ~Star();
    void Update(float delta, ShapeVec& shapeVec) override;          // 更新(deltaTime, ShapeVec)
    void Draw(void) override;                                       // 描画
    void Draw(float num) override;                                  // 描画(任意のサイズ変更可)
    void Draw(float num, Vector2Flt offset) override {};            // 描画(任意のサイズ変更可、図形の中心座標)
    ShapeType GetType() override { return ShapeType::Star; };       // shapeの種類を取得
    Vector2Flt GetPos() override { return pos_; };                  // 座標取得
    Vector2Flt GetSize() override { return size_; };                // サイズ取得
    int GetColor() override { return color_; };                     // 図形色を取得
};

