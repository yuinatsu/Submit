#pragma once
class SceneManager;

class SceneBase
{
public:

	SceneBase(SceneManager* manager);
	virtual ~SceneBase();

	// ����������
	virtual void Init(void);

	// �X�V�X�e�b�v
	virtual void Update(void);

	// �`�揈��
	virtual void Draw(void);

	// �������
	virtual void Release(void);

	SceneManager* GetSceneManager(void);

protected:
	SceneManager* sceneMng_;
};

