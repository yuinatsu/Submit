#include "Stage.h"

Stage::Stage(SceneManager* manager)
{
	sceneMng_ = manager;
}

void Stage::Init()
{
	modelID_ = MV1LoadModel("Model/Stage.mv1");
	pos_ = { 0.0f, -100.0f, 0.0f };

	// モデルの座標を設定
	MV1SetPosition(modelID_, pos_);
}

void Stage::Update()
{

}

void Stage::Draw()
{

	// X線
	DrawLine(0, 0, 100, 0, 0xffffff, true);
	// Y線
	DrawLine(0, 0, 0, 100, 0xffffff, true);
	// XYZのグリッド線描画
	auto LEN_LINE = 500.0f;
	for (int i = 0; i < 11; i++)
	{
		// X軸(赤)
		DrawLine3D(
			{ -LEN_LINE, 0.0f, -LEN_LINE + i * 100 },
			{ LEN_LINE, 0.0f, -LEN_LINE + i * 100 },
			0xff0000);

		// Z軸(青)
		DrawLine3D(
			{ -LEN_LINE + i * 100, 0.0f, LEN_LINE },
			{ -LEN_LINE + i * 100, 0.0f, -LEN_LINE },
			0x0000ff);
	}
	// Y軸(緑)
	DrawLine3D(
		{ 0.0f, LEN_LINE, 0.0f },
		{ 0.0f, -LEN_LINE, 0.0f },
		0x00ff00);

	// モデルを描画
	MV1DrawModel(modelID_);
}

void Stage::Release()
{
	MV1DeleteModel(modelID_);
}
