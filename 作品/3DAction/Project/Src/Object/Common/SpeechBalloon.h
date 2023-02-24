#pragma once
#include <string>
#include <DxLib.h>
#include "Transform.h"
class SceneManager;

class SpeechBalloon
{

public:

	enum class TYPE
	{
		TEXT,
		SPEECH
	};

	SpeechBalloon(SceneManager* manager, TYPE type, Transform* parent);

	void Draw(void);
	void DrawTextOrg(void);
	void DrawSpeech(void);
	void Release(void);

	void SetText(std::string text);
	void SetTime(double time);

	void SetType(TYPE type);
	void SetVisiblePermanent(void);

	void SetRelativePos(VECTOR relativePos);

private:

	SceneManager* mSceneManager;

	TYPE mType;

	int mImage;

	// true�̏ꍇ�A�i���I�ɕ\������
	bool mIsVisiblePermanent;
	double mVisibleTime;

	Transform* mTransformParent;
	VECTOR mPos;

	// �\���ʒu�̒����p
	VECTOR mRelativePos;

	std::string mText;

};
