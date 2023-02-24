#include <DxLib.h>
#include "Cursor.h"
#include "../Common/Input/Controller.h"
#include "../Common/ResourceMng.h"
#include "Button.h"

constexpr float cursorSize{ 20.0f };

Cursor::Cursor() :
	UiBase{zeroVector2<float>}
{
	lpResourceMng.LoadTexture(handle_, "Resource/resource/Ui/cursor.png");
}

void Cursor::Update(float delta, ObjectManager& objMng, Controller& controller)
{
	pos_ = controller.GetCursorPos();
}

void Cursor::Draw(void)
{
	DrawRotaGraphF(pos_.x, pos_.y, 1.0, 0.0, *handle_, true);
}

void Cursor::Check(std::list<std::unique_ptr<UiBase>>& uiList, BaseScene& scene,Controller& controller)
{
	auto checkpos = pos_ ;
	for (auto& ui : uiList)
	{
		ButtonBase* button;
		if (!ui->IsButton())
		{
			continue;
		}

		button = static_cast<ButtonBase*>(ui.get());
		if (ui->GetPos().x <= checkpos.x && (ui->GetPos().x + ui->GetSize().x) >= checkpos.x&&
			ui->GetPos().y <= checkpos.y && (ui->GetPos().y + ui->GetSize().y) >= checkpos.y
			)
		{
			if (!button->IsHitCursor())
			{
				// カーソルが乗ったことを伝える
				button->HitCursor();
			}

			if (controller.Press(InputID::Attack))
			{
				// クリックしたことを伝える
				button->Push(scene, controller);
			}
		

		}
		else
		{
			if (button->IsHitCursor())
			{
				// カーソルから離れたことを伝える
				button->NotHitCursor();
			}
		}
	}

	if (controller.Pressed(InputID::Attack))
	{
		return;
	}
}
