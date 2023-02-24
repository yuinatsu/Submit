#pragma once
/// <summary>
/// シングルトンクラス
/// </summary>
class Singleton
{
public:
	/// <summary>
	/// publicに自分の参照を返す関数を作る
	/// </summary>
	/// <returns></returns>
	static Singleton& Instance() {
		// ローカルstatic変数を作る
		static Singleton instance;
		return instance;
	}
	void OutputLife();
private:
	int life_;
	Singleton() {};// プライベートにコンストラクタ
	// コピー・代入禁止
	Singleton(const Singleton&) = delete;
	void operator=(const Singleton&) = delete;
};

