#include <fstream>
#include <DxLib.h>
#include <vector>
#include "InputConfigMng.h"
#include "InputConfig.h"
#include "../../Debug.h"

constexpr char path[] = "Resource/Other/Input/";
InputConfig* InputConfig::sInstance_ = nullptr;

// PadType���L�[�ɍ��킹�ăR���t�B�O�t�@�C���̖��O�ƃf�t�H���g�ɖ߂��֐������e�[�u��
InputConfig::TypeDataTbl InputConfig::typeDataTbl_{
	{DX_PADTYPE_DUAL_SHOCK_4,std::make_pair("padConfigPS.data",&InputConfig::SetPsDefalutCode)},
	{DX_PADTYPE_DUAL_SENSE,std::make_pair("padConfigPS.data",&InputConfig::SetPsDefalutCode)},
	{DX_PADTYPE_XBOX_360, std::make_pair("padConfigXbox.data", &InputConfig::SetXboxDefalutCode)},
	{DX_PADTYPE_XBOX_ONE, std::make_pair("padConfigXbox.data", &InputConfig::SetXboxDefalutCode)},
	{-1, std::make_pair("KeyboardConfig.data", &InputConfig::SetKeyDefalutCode)}
};

void InputConfig::Create(void)
{
	if (sInstance_ == nullptr)
	{
		sInstance_ = new InputConfig();
	}
}

void InputConfig::Destroy(void)
{
	if (sInstance_ != nullptr)
	{
		delete sInstance_;
	}
	sInstance_ = nullptr;
}

InputConfig& InputConfig::GetInstance(void)
{
	return *sInstance_;
}



void InputConfig::ChangeInputCode(InputID id, int code)
{
	// �ύX�O�̒l�������Ƃ�
	auto tmp = inputCode_[id];
	inputCode_[id] = code;
	auto itr = std::find_if(inputCode_.begin(), inputCode_.end(), [code](auto& data) { return data.second == code; });
	if (itr != inputCode_.end())
	{
		// �ύX�O�̒l������
		itr->second = tmp;
	}
}

void InputConfig::SetDefalutCode(void)
{
	if (typeDataTbl_.contains(nowType_))
	{
		(this->*typeDataTbl_[nowType_].second)();
	}
	else
	{
		SetKeyDefalutCode();
	}
}

void InputConfig::SetPsDefalutCode(void)
{
	inputCode_.emplace(InputID::Attack, 1);
	inputCode_.emplace(InputID::Dash, 0);
	inputCode_.emplace(InputID::Skil, 3);
	inputCode_.emplace(InputID::btn1, 9);
	inputCode_.emplace(InputID::Jump, 0);

}

void InputConfig::SetXboxDefalutCode(void)
{
	inputCode_.emplace(InputID::Attack, 0);
	inputCode_.emplace(InputID::btn1, 7);
	inputCode_.emplace(InputID::Dash, 1);
	inputCode_.emplace(InputID::Skil, 3);
	inputCode_.emplace(InputID::Jump, 2);
}

void InputConfig::SetKeyDefalutCode(void)
{
	inputCode_.emplace(InputID::Dash, KEY_INPUT_LSHIFT);
	inputCode_.emplace(InputID::Skil, KEY_INPUT_E);
	inputCode_.emplace(InputID::Attack, -MOUSE_INPUT_LEFT);
	inputCode_.emplace(InputID::Jump, KEY_INPUT_SPACE);
	inputCode_.emplace(InputID::btn1, KEY_INPUT_ESCAPE);
}

InputConfig::InputConfig()
{
	nowType_ = GetJoypadType(DX_INPUT_PAD1);

	if (typeDataTbl_.contains(nowType_))
	{
		if (!Load(typeDataTbl_[nowType_].first))
		{
			(this->*typeDataTbl_[nowType_].second)();
		}
	}
	else
	{
		nowType_ = -1;
		SetKeyDefalutCode();
	}
}

InputConfig::~InputConfig()
{
	if (typeDataTbl_.contains(nowType_))
	{
		Save(typeDataTbl_[nowType_].first);
	}
}

bool InputConfig::Load(const std::string& fname)
{
	std::ifstream ifs(path + fname);
	if (!ifs)
	{
		return false;
	}
	Header h{};
	std::vector<Data> vec;
	// �w�b�_�[����ǂݍ���
	ifs.read(reinterpret_cast<char*>(&h), sizeof(h));
	// �ݒ�f�[�^��ǂݍ���
	vec.resize(h.size);
	ifs.read(reinterpret_cast<char*>(vec.data()), sizeof(vec[0]) * h.size);
	int sum = 0;
	// �ȉ�sum�l�`�F�b�N
	for (auto& v : vec)
	{
		sum += static_cast<int>(v.id) * v.code;
	}
	if (sum != h.sum)
	{
		return false;
	}
	// �}�b�v�ɓ����
	for (auto& v : vec)
	{
		inputCode_.emplace(v.id, v.code);
	}
	return true;
}

bool InputConfig::Save(const std::string& fname)
{
	std::ofstream ofs(path + fname);
	if (!ofs)
	{
		return false;
	}
	Header h{};
	std::vector<Data> vec;
	//std::vector<Data> vec(inputCode_.size());
	vec.reserve(inputCode_.size());
	for (auto& code : inputCode_)
	{
		vec.emplace_back(Data{ code.first, code.second });
	}

	// �w�b�_�������
	h.size = vec.size();
	h.ver = 1.0f;
	for (auto& v : vec)
	{
		h.sum += static_cast<int>(v.id) * v.code;
	}
	// ��������
	ofs.write(reinterpret_cast<char*>(&h), sizeof(h));
	ofs.write(reinterpret_cast<char*>(vec.data()), sizeof(vec[0]) * vec.size());
	return true;
}
