#pragma once
#include "SceneBase.h"

class SpaceDome;
class TextScroll;
class ParticleGenerator;

class TitleScene : public SceneBase
{

public:

	TitleScene(SceneManager* manager);
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void DrawLogo(void);
	void Release(void) override;

private:
	TextScroll* textScroll_;
	int mImgStartLogo;
	SpaceDome* mSpaceDome;
	ParticleGenerator* particle_;
};

