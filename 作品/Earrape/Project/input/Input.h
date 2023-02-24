#pragma once
#include <map>
#include <string>

// 入力情報
using InputData = std::map<std::string, int>;

// 入力タイプ
enum class InputType
{
	NOW,	// 現在の入力情報
	OLD		// 過去の入力情報
};

class Obj;

class Input
{
public:
	Input();
	virtual ~Input();
	virtual bool Update(void) = 0;				// 更新
	const InputData& GetData(InputType type);	// 入力データ取得
private:
protected:
	InputData data_;							// 現在の入力
	InputData dataOld_;							// 前の入力
};
