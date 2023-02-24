#include <DxLib.h>
#include "Mouse.h"
#include "../_debug/_DebugConOut.h"

Mouse::Mouse()
{
	Init();			// ������
}

Mouse::Mouse(Vector2 offset)
{
	Init();			// ������
	offset_ = offset;
}

Mouse::~Mouse()
{
}

int Mouse::GetPresCnt(int keyCode)
{
	// �L���ȃL�[����Ȃ���Δ�����
	if (!isValidKeycode(keyCode))
	{
		return -1;
	}
	// �����Ă���Ԃ̃t���[����
	return btnPresCnt_[keyCode];
}

int Mouse::GetRelCnt(int keyCode)
{
	// �L���ȃL�[����Ȃ���Δ�����
	if (!isValidKeycode(keyCode))
	{
		return -1;
	}
	// �����Ă���Ԃ̃t���[����
	return btnRelCnt_[keyCode];
}

bool Mouse::Update(void)
{
	// �}�E�X�|�C���^�̈ʒu���擾����
	GetMousePoint(&data_["X���W"], &data_["Y���W"]);
	// �I�t�Z�b�g�l���Z
	data_["X���W"] -= offset_.x;
	data_["Y���W"] -= offset_.y;

	data_["�{�^��"] = GetMouseInput();			// �}�E�X�̃{�^���̉�����Ԃ��擾����
	data_["�z�C�[��"] = GetMouseWheelRotVol();	// �����}�E�X�z�C�[���̉�]�ʂ��擾����

	for (int i = 0; i < KEY_NUM; i++)
	{
		if ((GetMouseInput() >> i) & 1)
		{
			//i�Ԃ̃{�^����������Ă�����
			if (btnRelCnt_[i] > 0)
			{
				clickkCnt_++;
				//������J�E���^��0���傫�����
				btnRelCnt_[i] = 0;	//0�ɖ߂�
			}
			btnPresCnt_[i]++;	//������J�E���^�𑝂₷
		}
		else
		{
			//i�Ԃ̃L�[��������Ă�����
			if (btnPresCnt_[i] > 0)
			{
				//������J�E���^��0���傫�����
				btnPresCnt_[i] = 0;	//0�ɖ߂�
			}
			btnRelCnt_[i]++;	//������J�E���^�𑝂₷
		}
	}
	return true;
}

bool Mouse::Init(void)
{
	data_["X���W"]    = 0;
	data_["Y���W"]    = 0;
	data_["�{�^��"]   = 0;
	data_["�z�C�[��"] = 0;
	return true;
}

bool Mouse::isValidKeycode(int keyCode)
{
	if (!(0 <= keyCode && keyCode < KEY_NUM))
	{
		return false;
	}
	return true;
}
