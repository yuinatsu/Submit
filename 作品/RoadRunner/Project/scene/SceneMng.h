#pragma once
#include <memory>
#include "BaseScene.h"
#include "../common/Vector2.h"

#define lpSceneMng SceneMng::GetInstance()		// �C���X�^���X�擾

class SceneMng
{
public:
	// �C���X�^���X�擾
	static SceneMng& GetInstance()
	{
		static SceneMng s_Instance;			// �C���X�^���X����
		return s_Instance;
	}
	void Run(void);							// �Q�[�����[�v
	void Draw(void);						// �`��
	const Vector2 scnArea_;					// ��ʉ𑜓x
	const Vector2 worldArea_;				// �Q�[���̐��E�̃`�b�v��
	const Vector2 viewArea_;				// �\���`�b�v��
	const Vector2 chipSize_;				// �P�`�b�v������̃h�b�g��
	const Vector2 drawOffset_;				// ��ʍ��ォ��̃Q�[���G���A
private:
	uniqueBaseScene scene_;					// �V�[�����		// BaseScene* scene_ �� unique_ptr���܂߂�
	void Init(void);						// ������
	bool SysInit(void);						// �V�X�e��������
	SceneMng();
	~SceneMng();
};

