#pragma once
#include "Input.h"
#include "../common/Vector2.h"

#define lpMouse Mouse::GetInstance()

class Mouse :
	public Input
{
public:
	static Mouse& GetInstance(void)
	{
		static Mouse s_Instance;
		return s_Instance;
	}

	Mouse();
	Mouse(Vector2 offset);					// コンストラクタ(オフセット)
	~Mouse();
	int GetPresCnt(int keyCode);			// keyCodeのキーが押されているフレーム数を取得
	int GetRelCnt(int keyCode);				// keyCodeのキーが離されているフレーム数を取得
	bool Update(void) override;

	int clickkCnt_;							//全てのクリック回数、

	static constexpr int LMB = 0;			// マウスの左ボタン
	static constexpr int RMB = 1;			// マウスの右ボタン
private:
	bool Init(void);
	bool isValidKeycode(int keyCode);		// keyCodeが有効なキー番号か判断

	Vector2 offset_;
	static constexpr int KEY_NUM = 8;	//キー総数
	int btnPresCnt_[KEY_NUM];	//押されカウンタ
	int btnRelCnt_[KEY_NUM];	//離されカウンタ
};
