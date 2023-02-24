#include "ColorID.h"
#include "../../_debug/_DebugConOut.h"

void ColorID::Init(void)
{
	black_ = true;
	lightBlue_ = true;
	purple_ = true;
	yellow_ = true;
	green_ = true;
	red_ = true;
	orange_ = true;
	blue_ = true;
	door_ = true;
	color_ = Color::max;
	// ÉAÉCÉeÉÄèâä˙âª
	for (int m = 0; m < (static_cast<int>(Color::ItemMax) - static_cast<int>(Color::lightBlueItem)); m++)
	{
		itemFlag_[m] = true;
	}
}

bool ColorID::GetColor(Color color)
{
	if (color == Color::black)
	{
		return black_;
	}
	else if (color == Color::door)
	{
		return door_;
	}
	else if (color == Color::lightBlue)
	{
		return lightBlue_;
	}
	else if (color == Color::purple)
	{
		return purple_;
	}
	else if (color == Color::yellow)
	{
		return yellow_;
	}
	else if (color == Color::green)
	{
		return green_;
	}
	else if (color == Color::red)
	{
		return red_;
	}
	else if (color == Color::orange)
	{
		return orange_;
	}
	else if (color == Color::blue)
	{
		return blue_;
	}
	return true;
}

void ColorID::SetColor(Color color, bool flag)
{
	if (color == Color::black)
	{
		black_ = flag;
	}
	else if (color == Color::door)
	{
		door_ = flag;
	}
	else if (color == Color::lightBlue)
	{
		lightBlue_ = flag;
	}
	else if (color == Color::purple)
	{
		purple_ = flag;
	}
	else if (color == Color::yellow)
	{
		yellow_ = flag;
	}
	else if (color == Color::green)
	{
		green_ = flag;
	}
	else if (color == Color::red)
	{
		red_ = flag;
	}
	else if (color == Color::orange)
	{
		orange_ = flag;
	}
	else if (color == Color::blue)
	{
		blue_ = flag;
	}
}

Color ColorID::GetNowColor()
{
	return color_;
}

void ColorID::SetNowColor(Color color)
{
	color_ = color;
}

bool ColorID::GetItemFlag(Color color)
{
	if (color == Color::lightBlueItem)
	{
		return itemFlag_[0];
	}
	else if (color == Color::purpleItem)
	{
		return itemFlag_[1];
	}
	else if (color == Color::yellowItem)
	{
		return itemFlag_[2];
	}
	else if (color == Color::greenItem)
	{
		return itemFlag_[3];
	}
	else if (color == Color::redItem)
	{
		return itemFlag_[4];
	}
	else if (color == Color::orangeItem)
	{
		return itemFlag_[5];
	}
	else if (color == Color::blueItem)
	{
		return itemFlag_[6];
	}
	return true;
}

void ColorID::SetItemFlag(Color color, bool flag)
{
	if (color == Color::lightBlueItem)
	{
		itemFlag_[0] = flag;
	}
	else if (color == Color::purpleItem)
	{
		itemFlag_[1] = flag;
	}
	else if (color == Color::yellowItem)
	{
		itemFlag_[2] = flag;
	}
	else if (color == Color::greenItem)
	{
		itemFlag_[3] = flag;
	}
	else if (color == Color::redItem)
	{
		itemFlag_[4] = flag;
	}
	else if (color == Color::orangeItem)
	{
		itemFlag_[5] = flag;
	}
	else if (color == Color::blueItem)
	{
		itemFlag_[6] = flag;
	}
}

ColorID::ColorID()
{
	Init();
}
