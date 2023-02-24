#pragma once

class Fader
{

public:

	static constexpr float SPEED_ALPHA = 5;

	enum class FADE_STATE
	{
		NONE
		, FADE_OUT		// ���X�ɈÓ]
		, FADE_IN		// ���X�ɖ��]
	};

	FADE_STATE GetState(void);
	bool IsEnd(void);
	void SetFade(FADE_STATE state);
	
	void Init(void);
	void Update(void);
	void Draw(void);

private:

	FADE_STATE mState;

	// �����x
	int mAlpha;

	bool mIsEnd;

};

