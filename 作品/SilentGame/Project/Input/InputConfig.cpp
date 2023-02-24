#include <fstream>
#include <DxLib.h>
#include <vector>
#include "InputConfigMng.h"
#include "InputConfig.h"

constexpr char path[] = "Resource/Input/";
InputConfig* InputConfig::sInstance_ = nullptr;

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

const InputCode& InputConfig::GetKeyInputCode(void)
{
    return keyInputCode_;
}

void InputConfig::SetKeyInputCode(InputID id, int code)
{
    keyInputCode_[id] = code;
}

const InputCode& InputConfig::GetJoypadInputCode(void)
{
    return joypadInputCode_;
}

void InputConfig::SetJoypadInputCode(InputID id, int code)
{
    joypadInputCode_[id] = code;
}

const InputCode& InputConfig::GetJoypadInputMove(void)
{
    return joypadInputMove_;
}

void InputConfig::SetDefalutPadCode(void)
{
    auto type = GetJoypadType(DX_INPUT_PAD1);
    if (type == 3 || type == 4)
    {
        if (!Load("padConfigPS.data", joypadInputCode_))
        {
            joypadInputCode_.emplace(InputID::Attack, 1);
            joypadInputCode_.emplace(InputID::Crouch, 0);
            joypadInputCode_.emplace(InputID::Dash, 2);
            joypadInputCode_.emplace(InputID::ItemLeft, 4);
            joypadInputCode_.emplace(InputID::ItemRight, 5);
            joypadInputCode_.emplace(InputID::btn1, 9);
        }
    }
    else
    {
        if (!Load("padConfig.data", joypadInputCode_))
        {
            joypadInputCode_.emplace(InputID::Attack, 0);
            joypadInputCode_.emplace(InputID::Crouch, 1);
            joypadInputCode_.emplace(InputID::Dash, 2);
            joypadInputCode_.emplace(InputID::ItemLeft, 4);
            joypadInputCode_.emplace(InputID::ItemRight, 5);
            joypadInputCode_.emplace(InputID::btn1, 7);
        }
    }
}

void InputConfig::SetDefalutKeyCode(void)
{
    keyInputCode_[InputID::Attack] = KEY_INPUT_Z;
    keyInputCode_[InputID::Crouch] = KEY_INPUT_LCONTROL;
    keyInputCode_[InputID::Dash] = KEY_INPUT_LSHIFT;
    keyInputCode_[InputID::ItemLeft] = KEY_INPUT_X;
    keyInputCode_[InputID::ItemRight] = KEY_INPUT_C;
    keyInputCode_[InputID::btn1] = KEY_INPUT_ESCAPE;
}

void InputConfig::SwapKeyInputCode(InputID id, int code)
{
    int tmpCode{ keyInputCode_[id] };
    for (auto& keyCode : keyInputCode_)
    {
        if (keyCode.second == code)
        {
            keyCode.second = tmpCode;
        }
    }
    keyInputCode_[id] = code;
}

void InputConfig::SwapPadInputCode(InputID id, int code)
{
    int tmpCode{ joypadInputCode_[id] };
    for (auto& padCode : joypadInputCode_)
    {
        if (padCode.second == code)
        {
            padCode.second = tmpCode;
        }
    }
    joypadInputCode_[id] = code;
}

InputConfig::InputConfig()
{
    keyInputCode_.emplace(InputID::Up, KEY_INPUT_UP);
    keyInputCode_.emplace(InputID::Down, KEY_INPUT_DOWN);
    keyInputCode_.emplace(InputID::Left, KEY_INPUT_LEFT);
    keyInputCode_.emplace(InputID::Right, KEY_INPUT_RIGHT);

    joypadInputMove_.emplace(InputID::Up, PAD_INPUT_UP);
    joypadInputMove_.emplace(InputID::Down, PAD_INPUT_DOWN);
    joypadInputMove_.emplace(InputID::Left, PAD_INPUT_LEFT);
    joypadInputMove_.emplace(InputID::Right, PAD_INPUT_RIGHT);

    // �ݒ�t�@�C����ǂݍ���
    if (!Load("KeyboardConfig.data", keyInputCode_))
    {
        // �L�[�{�[�h�ݒ�̐ݒ�t�@�C�����ǂݍ��߂Ȃ������Ƃ�
        keyInputCode_.emplace(InputID::Attack, KEY_INPUT_Z);
        keyInputCode_.emplace(InputID::Crouch, KEY_INPUT_LCONTROL);
        keyInputCode_.emplace(InputID::Dash, KEY_INPUT_LSHIFT);
        keyInputCode_.emplace(InputID::ItemLeft, KEY_INPUT_X);
        keyInputCode_.emplace(InputID::ItemRight, KEY_INPUT_C);
        keyInputCode_.emplace(InputID::btn1, KEY_INPUT_ESCAPE);
    }
    auto type = GetJoypadType(DX_INPUT_PAD1);
    if (type == 3 || type == 4)
    {
        if (!Load("padConfigPS.data", joypadInputCode_))
        {
            joypadInputCode_.emplace(InputID::Attack, 1);
            joypadInputCode_.emplace(InputID::Crouch, 0);
            joypadInputCode_.emplace(InputID::Dash, 2);
            joypadInputCode_.emplace(InputID::ItemLeft, 4);
            joypadInputCode_.emplace(InputID::ItemRight, 5);
            joypadInputCode_.emplace(InputID::btn1, 9);
        }
    }
    else
    {
        if (!Load("padConfig.data", joypadInputCode_))
        {
            joypadInputCode_.emplace(InputID::Attack, 0);
            joypadInputCode_.emplace(InputID::Crouch, 1);
            joypadInputCode_.emplace(InputID::Dash, 2);
            joypadInputCode_.emplace(InputID::ItemLeft, 4);
            joypadInputCode_.emplace(InputID::ItemRight, 5);
            joypadInputCode_.emplace(InputID::btn1, 7);
        }
    }
}

InputConfig::~InputConfig()
{
    Save("KeyboardConfig.data", keyInputCode_);
    auto type = GetJoypadType(DX_INPUT_PAD1);
    if (type == 3 || type == 4)
    {
        Save("padConfigPS.data", joypadInputCode_);
    }
    else
    {
        Save("padConfig.data", joypadInputCode_);
    }
}

bool InputConfig::Load(const std::string& fname, InputCode& in)
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
        in.emplace(v.id, v.code);
    }
    return true;
}

bool InputConfig::Save(const std::string& fname, InputCode& in)
{
    std::ofstream ofs(path + fname);
    if (!ofs)
    {
        return false;
    }
    Header h{};
    std::vector<Data> vec;

    // �}�b�v����x�N�^�[�Ɋi�[����
    for (InputID id = InputID::Dash; static_cast<int>(id) < static_cast<int>(InputID::Max); id = static_cast<InputID>((static_cast<int>(id) + 1)))
    {
        vec.push_back({ id,in[id] });
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
