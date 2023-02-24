#pragma once
#include <map>
#include <unordered_map>
#include <string>
#include "../InputID.h"

#define lpConfigMng (InputConfig::GetInstance())

using InputCodeTbl = std::map<std::string, int>;
using InputCode = std::map<InputID, int>;

class InputConfig
{
	using TypeDataTbl = std::unordered_map<int, std::pair<std::string, void(InputConfig::*)(void)>>;
public:
	static void Create(void);
	static void Destroy(void);
	static InputConfig& GetInstance(void);


	/// <summary>
	/// 指定のIDのコードを変更する(被っている場合は入れ替わる)
	/// </summary>
	/// <param name="id"> 変更したいID </param>
	/// <param name="code"> 変更する値 </param>
	void ChangeInputCode(InputID id, int code);

	/// <summary>
	/// デフォルトの設定に戻す
	/// </summary>
	/// <param name=""></param>
	void SetDefalutCode(void);

	

	/// <summary>
	/// 設定されたcontrollerで使用するコードを取得
	/// </summary>
	/// <param name=""></param>
	/// <returns> コード </returns>
	const InputCode& GetInputCode(void) const&
	{
		return inputCode_;
	}

	void SetInputCode(InputCode& code)
	{
		inputCode_ = code;
	}

	/// <summary>
	/// 現在の入力タイプをかえす
	/// </summary>
	/// <param name=""></param>
	/// <returns> 現在の入力タイプ(int) </returns>
	const int GetNowType(void) const
	{
		return nowType_;
	}
private:
	InputConfig();
	~InputConfig();
	InputConfig(const InputConfig&) = delete;
	void operator=(const InputConfig&) = delete;

	/// <summary>
	/// プレイステーション系用のデフォルトに戻す処理
	/// </summary>
	/// <param name=""></param>
	void SetPsDefalutCode(void);

	/// <summary>
	/// Xbox系用のデフォルトに戻る処理
	/// </summary>
	/// <param name=""></param>
	void SetXboxDefalutCode(void);

	/// <summary>
	/// キーボード用のデフォルトに戻す処理
	/// </summary>
	/// <param name=""></param>
	void SetKeyDefalutCode(void);

	// ゲーム開始時にロードされる
	bool Load(const std::string& fname);

	// ゲーム終了時に現在使われているキーコードに変更される(セーブされる)
	bool Save(const std::string& fname);
	static InputConfig* sInstance_;

	// コード
	InputCode inputCode_;

	// 現在の入力タイプ
	int nowType_;

	// タイプ別に設定ファイル名とデフォルトに戻す関数をまとめたテーブル
	static TypeDataTbl typeDataTbl_;
};

