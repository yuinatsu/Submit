#pragma once
#include <DxLib.h>
#include <vector>
#include <memory>
#include "../common/Math.h"

class TmxObj;

// 足跡(1巡回分)
class Footprints
{
public:
	Footprints(std::vector<Math::Vector2I>& route,std::shared_ptr<TmxObj>& tmxObj);
	~Footprints();
	void Draw(const Math::Vector2& offset);
private:
	
	/// <summary> 開始座標と終了座標から頂点とindexをセットする </summary>
	/// <param name="start"> 開始座標 </param>
	/// <param name="end"> 終了座標 </param>
	void SetVertexAndIndex(const Math::Vector2& start, const Math::Vector2& end);

	// 頂点
	std::vector<VERTEX2D> vert_;

	// index
	std::vector<unsigned short> ind_;

	// 足跡一ラインがいくつあるかの数
	int num_;
};

