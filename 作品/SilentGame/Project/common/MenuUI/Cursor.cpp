#include <algorithm>
#include <DxLib.h>
#include "Cursor.h"

Cursor::Cursor(float thickness) :
	time_{ 0.0f }, drawTime_{ 0.0f },thickness_{thickness}
{
	constexpr auto defVert = []() {
		return VERTEX2D{
			VECTOR{0.0f,0.0f,0.0f},
			1.0f,
			COLOR_U8{255,255,255,255},
			0.0f,
			0.0f,
		};
	};
	
	std::for_each(top_.begin(), top_.end(), [](auto& v) { v = defVert(); });
	std::for_each(bottom_.begin(), bottom_.end(), [](auto& v) { v = defVert(); });
	std::for_each(left_.begin(), left_.end(), [](auto& v) { v = defVert(); });
	std::for_each(right_.begin(), right_.end(), [](auto& v) { v = defVert(); });
}



Cursor::~Cursor()
{
}

void Cursor::Update(float delta)
{
	time_ += delta;
	if (time_ > 1.0f)
	{
		drawTime_ += delta;
		if ((drawTime_ ) > 1.0f)
		{
			drawTime_ = 0.0f;
		}
	}
}

void Cursor::Draw(const Math::Vector2& lt, const Math::Vector2& rb)
{
	if (drawTime_ < 0.5f)
	{
		top_[0].pos = VGet(lt.x - thickness_, lt.y - thickness_, 0.0f);
		top_[1].pos = VGet(rb.x + thickness_, lt.y - thickness_, 0.0f);
		top_[2].pos = VGet(lt.x - thickness_, lt.y , 0.0f);
		top_[3].pos = VGet(rb.x + thickness_, lt.y, 0.0f);

		bottom_[0].pos = VGet(lt.x - thickness_, rb.y , 0.0f);
		bottom_[1].pos = VGet(rb.x + thickness_, rb.y , 0.0f);
		bottom_[2].pos = VGet(lt.x - thickness_, rb.y + thickness_, 0.0f);
		bottom_[3].pos = VGet(rb.x + thickness_, rb.y + thickness_, 0.0f);

		left_[0].pos = VGet(lt.x - thickness_, lt.y - thickness_, 0.0f);
		left_[1].pos = VGet(lt.x , lt.y - thickness_, 0.0f);
		left_[2].pos = VGet(lt.x - thickness_, rb.y + thickness_, 0.0f);
		left_[3].pos = VGet(lt.x , rb.y + thickness_, 0.0f);

		right_[0].pos = VGet(rb.x , lt.y - thickness_, 0.0f);
		right_[1].pos = VGet(rb.x + thickness_, lt.y - thickness_, 0.0f);
		right_[2].pos = VGet(rb.x , rb.y + thickness_, 0.0f);
		right_[3].pos = VGet(rb.x + thickness_, rb.y + thickness_, 0.0f);

		
		DrawPrimitive2D(top_.data(), 4, DX_PRIMTYPE_TRIANGLESTRIP, DX_NONE_GRAPH, true);
		DrawPrimitive2D(bottom_.data(), 4, DX_PRIMTYPE_TRIANGLESTRIP, DX_NONE_GRAPH, true);
		DrawPrimitive2D(left_.data(), 4, DX_PRIMTYPE_TRIANGLESTRIP, DX_NONE_GRAPH, true);
		DrawPrimitive2D(right_.data(), 4, DX_PRIMTYPE_TRIANGLESTRIP, DX_NONE_GRAPH, true);
		
	}
}

void Cursor::ResetBlink(void)
{
	time_ = 0.0f;
	drawTime_ = 0.0f;
}

void Cursor::Relese(void)
{
	
}
