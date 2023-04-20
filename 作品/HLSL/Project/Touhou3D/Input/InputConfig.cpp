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

void InputConfig::SetDefalutKeyCode(void)
{
    keyInputCode_[InputID::Decision] = KEY_INPUT_Z;
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

InputConfig::InputConfig()
{
    keyInputCode_.emplace(InputID::Up, KEY_INPUT_W);
    keyInputCode_.emplace(InputID::Down, KEY_INPUT_S);
    keyInputCode_.emplace(InputID::Left, KEY_INPUT_A);
    keyInputCode_.emplace(InputID::Right, KEY_INPUT_D);
    keyInputCode_.emplace(InputID::CameraUp, KEY_INPUT_UP);
    keyInputCode_.emplace(InputID::CameraDown, KEY_INPUT_DOWN);
    keyInputCode_.emplace(InputID::CameraLeft, KEY_INPUT_LEFT);
    keyInputCode_.emplace(InputID::CameraRight, KEY_INPUT_RIGHT);
    keyInputCode_.emplace(InputID::Dash, KEY_INPUT_LSHIFT);
    keyInputCode_.emplace(InputID::Dimension, KEY_INPUT_LCONTROL);

    // 設定ファイルを読み込み
    if (!Load("KeyboardConfig.data", keyInputCode_))
    {
        // キーボード設定の設定ファイルが読み込めなかったとき
        SetDefalutKeyCode();
    }
}

InputConfig::~InputConfig()
{
    Save("KeyboardConfig.data", keyInputCode_);
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
    // ヘッダー情報を読み込む
    ifs.read(reinterpret_cast<char*>(&h), sizeof(h));
    // 設定データを読み込む
    vec.resize(h.size);
    ifs.read(reinterpret_cast<char*>(vec.data()), sizeof(vec[0]) * h.size);
    int sum = 0;
    // 以下sum値チェック
    for (auto& v : vec)
    {
        sum += static_cast<int>(v.id) * v.code;
    }
    if (sum != h.sum)
    {
        return false;
    }
    // マップに入れる
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

    // マップからベクターに格納する
    for (InputID id = InputID::Decision; static_cast<int>(id) < static_cast<int>(InputID::Max); id = static_cast<InputID>((static_cast<int>(id) + 1)))
    {
        vec.push_back({ id,in[id] });
    }

    // ヘッダ情報を作る
    h.size = vec.size();
    h.ver = 1.0f;
    for (auto& v : vec)
    {
        h.sum += static_cast<int>(v.id) * v.code;
    }
    // 書き込み
    ofs.write(reinterpret_cast<char*>(&h), sizeof(h));
    ofs.write(reinterpret_cast<char*>(vec.data()), sizeof(vec[0]) * vec.size());
    return true;
}
