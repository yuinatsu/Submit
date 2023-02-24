#pragma once
#include <dxlib.h>
#include <vector>
#include <memory>
class SceneManager;
class Bullet;

// �A�j���[�V�������x
static constexpr float SPEED_ANIM = 20.0f;

// ���X�ɉ�]�����
static constexpr float SPEED_ROT = 5.0f;
static constexpr float SPEED_ROT_RAD = SPEED_ROT * (DX_PI_F / 180.0f);

class Unit
{
public:
	Unit(SceneManager* manager);
	void Init();
	void Update();
	void Draw();
	void Release();

	VECTOR GetPos(void);
	VECTOR GetAngles(void);

	char keyBuf[256];				// ���݂̃L�[�o�b�t�@
	char keyBufOld[256];			// �P�O�̃L�[�o�b�t�@
private:
	SceneManager* sceneMng_;
	std::vector<std::shared_ptr<Bullet>> bulet_;

	// ���f���̃n���h��ID
	int modelID_;

	// �A�j���[�V�����p�̃n���h��ID
	int animWalkID_;

	// ���f���̕\�����W
	VECTOR pos_;

	// ���f���̊p�x
	VECTOR angles_;
	VECTOR anglesLocal_;

	// �A�j���[�V�����̑��Đ�����
	float timeTotalAnimWalk_;

	// �A�j���[�V�����p�̍X�V�X�e�b�v
	float stepAnim_;
};

