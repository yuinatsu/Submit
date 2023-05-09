#pragma once
#include <map>
#include <string>
#include "InputID.h"

#define lpConfigMng (InputConfig::GetInstance())

using InputCodeTbl = std::map<std::string, int>;
using InputCode = std::map<InputID, int>;

class InputConfig
{
public:
	static void Create(void);
	static void Destroy(void);
	static InputConfig& GetInstance(void);

	// キーボード用のコードを取得
	const InputCode& GetKeyInputCode(void);
	// キーボード用のコードの参照
	void SetKeyInputCode(InputID id, int code);
	// キーボードのコードをすべてデフォルトとする
	void SetDefalutKeyCode(void);
	// 設定しているキーコードを変更(キーボード用)
	void SwapKeyInputCode(InputID id, int code);
private:
	InputConfig();
	~InputConfig();
	InputConfig(const InputConfig&) = delete;
	void operator=(const InputConfig&) = delete;
	// ゲーム開始時にロードされる
	bool Load(const std::string& fname, InputCode& in);
	// ゲーム終了時に現在使われているキーコードに変更される(セーブされる)
	bool Save(const std::string& fname, InputCode& in);
	static InputConfig* sInstance_;
	InputCode keyInputCode_;
	InputCode joypadInputCode_;
	InputCode joypadInputMove_;
};

