#pragma once
#include "Input.h"
#include "../common/Vector2.h"

#define lpMouse Mouse::GetInstance()

class Mouse :
	public Input
{
public:
	static Mouse& GetInstance(void)
	{
		static Mouse s_Instance;
		return s_Instance;
	}

	Mouse();
	Mouse(Vector2 offset);					// �R���X�g���N�^(�I�t�Z�b�g)
	~Mouse();
	int GetPresCnt(int keyCode);			// keyCode�̃L�[��������Ă���t���[�������擾
	int GetRelCnt(int keyCode);				// keyCode�̃L�[��������Ă���t���[�������擾
	bool Update(void) override;

	int clickkCnt_;							//�S�ẴN���b�N�񐔁A

	static constexpr int LMB = 0;			// �}�E�X�̍��{�^��
	static constexpr int RMB = 1;			// �}�E�X�̉E�{�^��
private:
	bool Init(void);
	bool isValidKeycode(int keyCode);		// keyCode���L���ȃL�[�ԍ������f

	Vector2 offset_;
	static constexpr int KEY_NUM = 8;	//�L�[����
	int btnPresCnt_[KEY_NUM];	//������J�E���^
	int btnRelCnt_[KEY_NUM];	//������J�E���^
};
