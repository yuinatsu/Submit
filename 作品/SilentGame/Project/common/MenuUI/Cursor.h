#pragma once
#include <DxLib.h>
#include <array>
#include "../Math.h"

class Cursor
{
public:
	Cursor(float thickness);
	~Cursor();
	void Update(float delta);
	void Draw(const Math::Vector2& lt, const Math::Vector2& rb);
	void ResetBlink(void);
	void Relese(void);
private:
	std::array<VERTEX2D, 4> top_;

	std::array<VERTEX2D, 4> bottom_;

	std::array<VERTEX2D, 4> left_;

	std::array<VERTEX2D, 4> right_;


	// Œo‰ßŽžŠÔ
	float time_;

	// •`‰æŽžŠÔ
	float drawTime_;

	// ‘¾‚³
	float thickness_;
};

