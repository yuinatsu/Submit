#pragma once
#include <list>
#include <memory>
#include "BaseScene.h"
#include "../Obj/Obj.h"
#include "../../Tiled/TmxObj.h"
class TmxObj;
class Player;

class GameScene:
	public BaseScene
{
public:
	static constexpr int StartCallMax = 2;									// �J�E���g�_�E�����S����
	static constexpr double CallCountMax = 1.0;								// �J�E���g�_�E���̊Ԋu
	GameScene();
	~GameScene();
private:
	std::shared_ptr<TmxObj> tmxObj_;										// �X�e�[�W�f�[�^(tmx)
	std::list<std::unique_ptr<Obj>> objList_;								// Obj�̃��X�g

	bool Init(void) override;												// ������
	uniqueBaseScene Update(double delta, uniqueBaseScene own) override;		// �X�V Update(deltaTime, �e�V�[��)
	void DrawOwnScreen(double delta);										// �e�V�[���̕`��
	Scene GetSceneID(void) override { return Scene::Game; };				// �e�V�[���̎擾
	void CallDraw();													// �Q�[���J�n�̃J�E���g�_�E���`��

	bool bflag_;															// �{�^���t���O
	Stage stage_;															// �X�e�[�W���i�[�p
	int stageNum_;
	bool clearFlag_;														// �N���A����
	int pauseNum_;

	// �J�E���g�_�E��
	double count_;
	bool callFlag_;															// �R�[���̗L���̃t���O

	int clearCnt_;
	bool playerFlag_;

	int gameBGM1_;
	int gameBGM2_;
	int gameBGM3_;
	int selectSE_;
};

