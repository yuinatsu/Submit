#include <DxLib.h>
#include "Mouse.h"
#include "../../_debug/_DebugConOut.h"


Mouse::Mouse()
{
	//TRACE("Mouse�𐶐�\n");
	Init();
}


Mouse::Mouse(Vector2 offset)
{
	//TRACE("Mouse�𐶐�\n");
	Init();
	offset_ = offset;
}


Mouse::~Mouse()
{
}


bool Mouse::Update(Obj& obj)
{
	GetMousePoint(&data_["X���W"], &data_["Y���W"]);
	data_["X���W"] -= offset_.x;				// �Q�[�����̃I�t�Z�b�g�����炷
	data_["Y���W"] -= offset_.y;				// �Q�[�����̃I�t�Z�b�g�����炷
	data_["�{�^��"] = GetMouseInput() & MOUSE_INPUT_LEFT;
	data_["�z�C�[��"] = GetMouseWheelRotVol();
	data_["���Z�b�g"] = CheckHitKey(KEY_INPUT_F9);
	data_["�Z�[�u"] = CheckHitKey(KEY_INPUT_F8);
	data_["���[�h"] = CheckHitKey(KEY_INPUT_F7);
	data_["�v���C"] = CheckHitKey(KEY_INPUT_F5);
	data_["�w���v"] = CheckHitKey(KEY_INPUT_F4);
	data_["�u���b�N�ꗗ"] = CheckHitKey(KEY_INPUT_F3);

	return true;
}


bool Mouse::Init()
{
	data_["X���W"] = 0;
	data_["Y���W"] = 0;
	data_["�{�^��"] = 0;
	data_["�z�C�[��"] = 0;
	data_["���Z�b�g"] = 0;
	data_["�Z�[�u"] = 0;
	data_["���[�h"] = 0;
	data_["�v���C"] = 0;
	data_["�w���v"] = 0;
	data_["�u���b�N�ꗗ"] = 0;

	return true;
}
