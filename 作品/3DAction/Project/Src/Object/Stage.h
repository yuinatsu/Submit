#pragma once
#include <map>
#include "Common/Transform.h"
class SceneManager;
class ResourceManager;
class WarpStar;
class Planet;
class Player;

class Stage
{

public:

	// �X�e�[�W�̐؂�ւ��Ԋu
	static constexpr float TIME_STAGE_CHANGE = 1.0f;

	enum class NAME
	{
		MAIN_PLANET,
		FALL_PLANET,
		FLAT_PLANET_BASE,
		FLAT_PLANET_ROT01,
		FLAT_PLANET_ROT02,
		FLAT_PLANET_ROT03,
		FLAT_PLANET_ROT04,
		FLAT_PLANET_FIXED01,
		FLAT_PLANET_FIXED02,
		PLANET10,
		LAST_STAGE,
		SPECIAL_STAGE
	};

	Stage(SceneManager* manager, Player* player);
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	// �X�e�[�W�ύX
	void ChangeStage(NAME type);

private:

	SceneManager* mSceneManager;
	ResourceManager* mResourceManager;

	Player* mPlayer;

	// �A�N�e�B�u�ȃX�e�[�W�̏��
	NAME mActiveName;
	Planet* mActivePlanet;

	// �f��
	std::map<NAME, Planet*> mPlanets;

	// ���[�v�X�^�[
	std::vector<WarpStar*> mWarpStars;

	float mStep;

	// �ŏ��̘f��
	void MakeMainStage(void);

	// ���Ƃ����f��
	void MakeFallPlanet(void);

	// ���R�Șf��
	void MakeFlatPlanet(void);

	// �Ō�̘f��
	void MakeLastPlanet(void);

	// �ԊO�҂̘f��
	void MakeSpecialPlanet(void);

	// ���[�v�X�^�[
	void MakeWarpStar(void);

	// �S�[���X�^�[
	void MakeGoalStar(void);

	// �ΏۃX�e�[�W���擾
	Planet* GetPlanet(NAME type);

};