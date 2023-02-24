#pragma once
#include <list>
#include <memory>

class ResultDraw;
class ResultData;

class ResultDrawCtrl
{
	using ResultDrawList = std::list<std::unique_ptr<ResultDraw>>;
public:

	/// <summary> リザルトの結果の描画をコントロールする </summary>
	/// <param name="data"> リザルトデータ </param>
	ResultDrawCtrl(ResultData& data,float delta);

	~ResultDrawCtrl();
	void Update(float delta);
	void Draw(void);

	const bool IsStartedDraw(void) const;
private:

	// 描画すべき結果のリスト
	ResultDrawList drawList_;

	// 経過時間
	float time_;
};

