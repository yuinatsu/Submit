#include "Stage.h"

Stage::Stage(SceneManager* manager)
{
	sceneMng_ = manager;
}

void Stage::Init()
{
	modelID_ = MV1LoadModel("Model/Stage.mv1");
	pos_ = { 0.0f, -100.0f, 0.0f };

	// ���f���̍��W��ݒ�
	MV1SetPosition(modelID_, pos_);
}

void Stage::Update()
{

}

void Stage::Draw()
{

	// X��
	DrawLine(0, 0, 100, 0, 0xffffff, true);
	// Y��
	DrawLine(0, 0, 0, 100, 0xffffff, true);
	// XYZ�̃O���b�h���`��
	auto LEN_LINE = 500.0f;
	for (int i = 0; i < 11; i++)
	{
		// X��(��)
		DrawLine3D(
			{ -LEN_LINE, 0.0f, -LEN_LINE + i * 100 },
			{ LEN_LINE, 0.0f, -LEN_LINE + i * 100 },
			0xff0000);

		// Z��(��)
		DrawLine3D(
			{ -LEN_LINE + i * 100, 0.0f, LEN_LINE },
			{ -LEN_LINE + i * 100, 0.0f, -LEN_LINE },
			0x0000ff);
	}
	// Y��(��)
	DrawLine3D(
		{ 0.0f, LEN_LINE, 0.0f },
		{ 0.0f, -LEN_LINE, 0.0f },
		0x00ff00);

	// ���f����`��
	MV1DrawModel(modelID_);
}

void Stage::Release()
{
	MV1DeleteModel(modelID_);
}
