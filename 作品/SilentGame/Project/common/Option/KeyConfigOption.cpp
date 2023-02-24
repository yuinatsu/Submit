#include <DxLib.h>
#include "../ImageMng.h"
#include "../../Input/InputConfig.h"
#include "../MenuUI/MenuUICtrl.h"
#include "KeyConfigOption.h"

KeyConfigOption::KeyConfigOption(Controller& controller) :
	OptionWindow{controller}
{
	key_.fill(0);
	oldkey_.fill(0);
	

	menuUICtrl_ = std::make_unique<MenuUICtrl<SettingType>>(
		controller_,
		static_cast<int>(SettingType::Max),
		3.0f,
		SettingType::Dash
		);

	menuUICtrl_->AddMenu(SettingType::Dash, std::make_unique<MenuUI>(Math::Vector2{ 500.0f,177.5f }, "Resource/Input/Dash.png"));
	menuUICtrl_->AddMenu(SettingType::Crouch, std::make_unique<MenuUI>(Math::Vector2{ 500.0f,227.5f }, "Resource/Input/Crouch.png"));
	menuUICtrl_->AddMenu(SettingType::ItemLeft, std::make_unique<MenuUI>(Math::Vector2{ 500.0f,277.5f }, "Resource/Input/ItemLeft.png"));
	menuUICtrl_->AddMenu(SettingType::ItemRight, std::make_unique<MenuUI>(Math::Vector2{ 500.0f,327.5f }, "Resource/Input/ItemRight.png"));
	menuUICtrl_->AddMenu(SettingType::Attack, std::make_unique<MenuUI>(Math::Vector2{ 500.0f,377.5f }, "Resource/Input/Attack.png"));
	//menuUICtrl_->AddMenu(SettingType::btn1, std::make_unique<MenuUI>(Math::Vector2{ 500.0f,437.5f }, "Resource/Input/Menu.png"));
	//menuUICtrl_->AddMenu(SettingType::Init, std::make_unique<MenuUI>(Math::Vector2{ 500.0f,467.5f }, "Resource/Input/Default.png"));
	//menuUICtrl_->AddMenu(SettingType::Exit, std::make_unique<MenuUI>(Math::Vector2{ 500.0f,517.5f }, "Resource/Input/Exit.png"));
	menuUICtrl_->AddMenu(SettingType::Init, std::make_unique<MenuUI>(Math::Vector2{ 500.0f,437.5f }, "Resource/Input/Default.png"));
	menuUICtrl_->AddMenu(SettingType::Exit, std::make_unique<MenuUI>(Math::Vector2{ 500.0f,477.5f }, "Resource/Input/Exit.png"));
	updata_ = std::bind(&KeyConfigOption::UpdateKeyConfig, this, std::placeholders::_1);
	draw_ = std::bind(&KeyConfigOption::DrawKeyConfig, this);
}

bool KeyConfigOption::Update(float delta)
{
	time_ += delta;
	return updata_(delta);
}

void KeyConfigOption::Draw(void)
{
	draw_();
}

bool KeyConfigOption::UpdateKeyConfig(float delta)
{
	menuUICtrl_->Update(delta);
	if (controller_.Pressed(InputID::Attack) && time_ > 0.2f)
	{
		bool flag = false;
		switch (menuUICtrl_->GetNowMenu())
		{
		case SettingType::Dash:
			inputID_ = InputID::Dash;
			flag = true;
			break;
		case SettingType::Crouch:
			inputID_ = InputID::Crouch;
			flag = true;
			break;
		case SettingType::ItemLeft:
			inputID_ = InputID::ItemLeft;
			flag = true;
			break;
		case SettingType::ItemRight:
			inputID_ = InputID::ItemRight;
			flag = true;
			break;
		case SettingType::Attack:
			inputID_ = InputID::Attack;
			flag = true;
			break;
		//case SettingType::btn1:
		//	inputID_ = InputID::btn1;
		//	flag = true;
		//	break;
		case SettingType::Init:
			lpConfigMng.SetDefalutKeyCode();
			lpConfigMng.SetDefalutPadCode();
			time_ = 0.0f;
			break;
		case SettingType::Exit:
			return false;
		default:
			break;
		}

		if (flag)
		{
			updata_ = std::bind(&KeyConfigOption::UpdateSet, this, std::placeholders::_1);
			draw_ = std::bind(&KeyConfigOption::DrawSet, this);
			time_ = 0.0f;
			isDrawInput_ = false;
		}
	}
	return true;
}

void KeyConfigOption::DrawKeyConfig(void)
{
	menuUICtrl_->Draw();
	if (controller_.GetCntType() == CntType::Key)
	{
		DrawKeyBoardConfig();
	}
	else
	{
		DrawPadConfig();
	}

	return;
}

void KeyConfigOption::DrawPadConfig(void)
{
	auto type = GetJoypadType(DX_INPUT_PAD1);
	if (type == 3 || type == 4)
	{
		auto& data = lpImageMng.GetID("PadPS");
		DrawGraph(800, static_cast<int>(menuUICtrl_->GetUI(SettingType::Dash).GetLT().y), data[lpConfigMng.GetJoypadInputCode().at(InputID::Dash)], true);
		DrawGraph(800, static_cast<int>(menuUICtrl_->GetUI(SettingType::Crouch).GetLT().y), data[lpConfigMng.GetJoypadInputCode().at(InputID::Crouch)], true);
		DrawGraph(800, static_cast<int>(menuUICtrl_->GetUI(SettingType::ItemLeft).GetLT().y), data[lpConfigMng.GetJoypadInputCode().at(InputID::ItemLeft)], true);
		DrawGraph(800, static_cast<int>(menuUICtrl_->GetUI(SettingType::ItemRight).GetLT().y), data[lpConfigMng.GetJoypadInputCode().at(InputID::ItemRight)], true);
		DrawGraph(800, static_cast<int>(menuUICtrl_->GetUI(SettingType::Attack).GetLT().y), data[lpConfigMng.GetJoypadInputCode().at(InputID::Attack)], true);
		//DrawGraph(800, menuUICtrl_->GetUI(SettingType::btn1).GetLT().y, data[lpConfigMng.GetJoypadInputCode().at(InputID::btn1)], true);
	}
	else
	{
		auto& data = lpImageMng.GetID("PadXbox");
		DrawGraph(800, static_cast<int>(menuUICtrl_->GetUI(SettingType::Dash).GetLT().y), data[lpConfigMng.GetJoypadInputCode().at(InputID::Dash)], true);
		DrawGraph(800, static_cast<int>(menuUICtrl_->GetUI(SettingType::Crouch).GetLT().y), data[lpConfigMng.GetJoypadInputCode().at(InputID::Crouch)], true);
		DrawGraph(800, static_cast<int>(menuUICtrl_->GetUI(SettingType::ItemLeft).GetLT().y), data[lpConfigMng.GetJoypadInputCode().at(InputID::ItemLeft)], true);
		DrawGraph(800, static_cast<int>(menuUICtrl_->GetUI(SettingType::ItemRight).GetLT().y), data[lpConfigMng.GetJoypadInputCode().at(InputID::ItemRight)], true);
		DrawGraph(800, static_cast<int>(menuUICtrl_->GetUI(SettingType::Attack).GetLT().y), data[lpConfigMng.GetJoypadInputCode().at(InputID::Attack)], true);
		//DrawGraph(800, menuUICtrl_->GetUI(SettingType::btn1).GetLT().y, data[lpConfigMng.GetJoypadInputCode().at(InputID::btn1)], true);
	}

}

void KeyConfigOption::DrawKeyBoardConfig(void)
{
	auto& data = lpImageMng.GetID("Keydata");
	DrawGraph(800, static_cast<int>(menuUICtrl_->GetUI(SettingType::Dash).GetLT().y), data[lpConfigMng.GetKeyInputCode().at(InputID::Dash)], true);
	DrawGraph(800, static_cast<int>(menuUICtrl_->GetUI(SettingType::Crouch).GetLT().y), data[lpConfigMng.GetKeyInputCode().at(InputID::Crouch)], true);
	DrawGraph(800, static_cast<int>(menuUICtrl_->GetUI(SettingType::ItemLeft).GetLT().y), data[lpConfigMng.GetKeyInputCode().at(InputID::ItemLeft)], true);
	DrawGraph(800, static_cast<int>(menuUICtrl_->GetUI(SettingType::ItemRight).GetLT().y), data[lpConfigMng.GetKeyInputCode().at(InputID::ItemRight)], true);
	DrawGraph(800, static_cast<int>(menuUICtrl_->GetUI(SettingType::Attack).GetLT().y), data[lpConfigMng.GetKeyInputCode().at(InputID::Attack)], true);
	//DrawGraph(800, menuUICtrl_->GetUI(SettingType::btn1).GetLT().y, data[lpConfigMng.GetKeyInputCode().at(InputID::btn1)], true);
}

bool KeyConfigOption::UpdateSet(float delta)
{
	if (controller_.GetCntType() == CntType::Key)
	{
		oldkey_ = key_;
		GetHitKeyStateAll(key_.data());
		for (int i = 0; i < key_.size(); i++)
		{
			if (i == KEY_INPUT_UP || i == KEY_INPUT_DOWN
				|| i == KEY_INPUT_RIGHT || i == KEY_INPUT_LEFT
				|| i == KEY_INPUT_ESCAPE || i > 53)
			{
				// 方向キーの時は飛ばすorキーの値が53以上だったら飛ばす
				continue;
			}

			if (key_[i] && !oldkey_[i] && time_ > 0.2f)
			{
				// iに対応するキーが押されたときそれをセットする
				lpConfigMng.SwapKeyInputCode(inputID_, i);
				updata_ = std::bind(&KeyConfigOption::UpdateKeyConfig, this, std::placeholders::_1);
				draw_ = std::bind(&KeyConfigOption::DrawKeyConfig, this);
				time_ = 0.0f;
				isDrawInput_ = true;
				return true;

			}
		}
	}
	else
	{
		GetJoypadDirectInputState(DX_INPUT_PAD1, &state_);

		for (int i = 0; i < std::size(state_.Buttons); i++)
		{
			if (state_.Buttons[i] && time_ > 0.2f)
			{
				// iに対応するボタンが押された時それをセットする
				lpConfigMng.SwapPadInputCode(inputID_, i);
				updata_ = std::bind(&KeyConfigOption::UpdateKeyConfig, this, std::placeholders::_1);
				draw_ = std::bind(&KeyConfigOption::DrawKeyConfig, this);
				time_ = 0.0f;
				isDrawInput_ = true;
				return true;
			}
		}
	}
	return true;
}

void KeyConfigOption::DrawSet(void)
{
	//DrawFormatString(600, 290, 0xffffff, "どのキーに変更しますか？");
	DrawGraph(600, 280, lpImageMng.GetID("./Resource/Image/Option/Confirmation.png")[0], true);
	if (controller_.GetCntType() == CntType::Key)
	{
		auto& data = lpImageMng.GetID("Keydata");
		DrawGraph(600, 320, data[lpConfigMng.GetKeyInputCode().at(inputID_)], true);
	}
	else
	{
		auto type = GetJoypadType(DX_INPUT_PAD1);
		if (type == 4 || type == 3)
		{
			auto& data = lpImageMng.GetID("PadPS");
			DrawGraph(600, 320, data[lpConfigMng.GetJoypadInputCode().at(inputID_)], true);
		}
		else
		{
			auto& data = lpImageMng.GetID("PadXbox");
			DrawGraph(600, 320, data[lpConfigMng.GetJoypadInputCode().at(inputID_)], true);
		}
	}
}
