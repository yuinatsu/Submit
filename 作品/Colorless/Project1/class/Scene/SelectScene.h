#pragma once
#include "BaseScene.h"
#include "../Obj/Obj.h"
#include "../../Tiled/TmxObj.h"
class TmxObj;
class Player;

class SelectScene :
    public BaseScene
{
public:
	static constexpr int CursorRangeUp = 150;								// �J�[�\���ړ������㕔
	static constexpr int CursorRangeDown = 550;								// �J�[�\���ړ��������
	SelectScene();
	~SelectScene();
private:
	bool Init(void) override;												// ������
	uniqueBaseScene Update(double delta, uniqueBaseScene own) override;		// �X�V Update(deltaTime, �e�V�[��)
	void DrawOwnScreen(double delta);										// �e�V�[���̕`��
	Scene GetSceneID(void) override { return Scene::Select; };				// �e�V�[���̎擾

	std::shared_ptr<TmxObj> tmxObj_;										// �X�e�[�W�f�[�^(tmx)
	std::list<std::unique_ptr<Obj>> obj_;									// Obj�̃��X�g
	bool selectFlag_;														// �Z���N�g����
	bool bflag_;															// �{�^���t���O
	int stageNum_;															// �X�e�[�W�i���o�[

	int pauseNum_;
	int startCnt_;
	double count_;																	// 1�t���[��������̒l
	bool playerFlag_;
	bool logoFlag_;

	int selectBGM_;
	int selectSE_;
};

