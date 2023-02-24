#pragma once

#define lpColorMng ColorID::GetInstance()

// 色の種類
enum class Color
{
	black,				// 黒
	door,				// ドア
	// ↓アイテム色
	lightBlueItem,		// 水色
	purpleItem,			// 紫
	yellowItem,			// 黄色
	greenItem,			// 緑
	redItem,			// 赤
	orangeItem,			// オレンジ
	blueItem,			// 青
	ItemMax,
	// ↓色変え可能色
	lightBlue,			// 水色
	purple,				// 紫
	yellow,				// 黄色
	green,				// 緑
	red,				// 赤
	orange,				// オレンジ
	blue,				// 青
	max,
};

class ColorID
{
public:
	static ColorID& GetInstance()
	{
		static ColorID s_Instance;
		return s_Instance;
	}
	void Init(void);								// 初期化
	bool GetColor(Color color);						// 色の有無の取得 GetColor(色の種類)
	void SetColor(Color color, bool flag);			// 色の有無のセット　SetColor(色の種類, フラグ)
	Color GetNowColor();							// 現在の背景色の取得
	void SetNowColor(Color color);					// 現在の背景色のセット
	bool GetItemFlag(Color color);
	void SetItemFlag(Color color, bool flag);
private:
	ColorID();
	// 色格納用変数
	bool black_;
	bool lightBlue_;
	bool purple_;
	bool yellow_;
	bool green_;
	bool red_;
	bool orange_;
	bool blue_;
	bool door_;
	// 現在の背景色
	Color color_;
	// アイテム関係
	bool itemFlag_[static_cast<int>(Color::ItemMax) - static_cast<int>(Color::lightBlueItem)];
};

