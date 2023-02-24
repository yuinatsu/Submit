#pragma once
#include "BaseScene.h"
#include "../SCN_ID.h"
#include "../common/Vector2.h"

enum class GuidePage
{
	PAGE_1,
	PAGE_2,
	PAGE_3,
	PAGE_4,
	PAGE_5,
	MAX
};

class GuideScene :
    public BaseScene
{
public:
	GuideScene();
	~GuideScene();
	SCN_ID Update(char* keyData, char* keyDataOld) override;
	void Draw(void) override;	// �O���t�B�b�N�̕`��
	bool Init(void) override;	// ������
	bool Release(void) override;	// ���\�[�X�̉��
private:
	Vector2 pos_;			//�}�E�X�̍��W
	int pageCnt_;			// �y�[�W�J�E���^�[
	int guideImage_[static_cast<int>(GuidePage::MAX)];	// ���[�������Ɋւ��Ă͉摜�ɕ��͓����܂Ƃ߂ĕ\������(�y�[�W�Ƃ��ĉ������ɕ�����)
};

