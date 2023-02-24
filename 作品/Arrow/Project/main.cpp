#include<DxLib.h>
#include<array>//stlで配列を表すもの
#include<cassert>
#include<memory>//スマートポインタ
#include<sstream>//文字列ストリーム
#include<iomanip>//ストリームマニピュレータ
#include"Debug.h"
#include"Game.h"
#include"Scene.h"
#include"Player.h"
#include"Enemy.h"
#include"Singleton.h"
#include"Geometry.h"


using KeyboardData_t = std::array<char, 256>;

struct Rect {
	Position2 center;
	float w;//width/2
	float h;//height/w
	float Left()const {
		return center.x - w;
	}
	float Top()const {
		return center.y - h;
	}
	float Right()const {
		return center.x + w;
	}
	float Bottom()const {
		return center.y + h;
	}
	float Width()const {
		return w * 2;
	}
	float Height()const {
		return h * 2;
	}
};

void MoveByInput(const KeyboardData_t& keystate, Position2& pos) {

	constexpr float speed = 4.0f;
	if (keystate[KEY_INPUT_LEFT]) {
		pos.x -= speed;
	}
	if (keystate[KEY_INPUT_RIGHT]) {
		pos.x += speed;
	}
	if (keystate[KEY_INPUT_UP]) {
		pos.y -= speed;
	}
	if (keystate[KEY_INPUT_DOWN]) {
		pos.y += speed;
	}
}

Position2 GetMousePosition2() {
	int mx, my;
	GetMousePoint(&mx, &my);
	return { static_cast<float>(mx),static_cast<float>(my) };
}

/// <summary>
/// 度数法を弧度法に変換
/// </summary>
/// <param name="deg">度数</param>
/// <returns>ラジアン</returns>
float DegreeToRadian(float deg) {
	return deg * DX_PI / 180.0f;
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	Singleton& singleton = Singleton::Instance();
	//↓コピーコンストラクタが有効になってる
	auto& singleton2 = Singleton::Instance();



	SetWindowText(L"2016010_梶原尚人");
	ChangeWindowMode(true);
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);

	//背景用
	int bgH[4];
	LoadDivGraph(L"img/bganim.png", 4, 4, 1, 256, 192, bgH);

	int groundH = LoadGraph(L"img/ground.png");
	assert(groundH >= 0);
	int bgAssetH = LoadGraph(L"img/Assets.png");
	assert(bgAssetH >= 0);
	int arrowH = LoadGraph(L"img/arrow2.png");
	assert(arrowH >= 0);

	constexpr size_t picture_num = 5;
	int graphH[picture_num] = {};
	for (int i = 0; i < picture_num; ++i) {
		std::wostringstream oss;
		//oss << L"../Asset/Adventurer-1.5/Individual Sprites/adventurer-run-";
		oss << L"../Asset/Adventurer-1.5/Individual Sprites/adventurer-attack1-";
		oss << std::setw(2);
		oss << std::setfill(L'0');
		oss << i << ".png";

		//wsprintf(path, L"../../Asset/Adventurer-1.5/Individual Sprites/adventurer-run-%02d.png", i);
		graphH[i] = LoadGraph(oss.str().c_str());
		assert(graphH[i] != -1);
	}
	Rect rcA = { {100,100},75,75 };
	KeyboardData_t keystate;

	int a[10] = {};
	a[5]++;

	a[7]++;
	constexpr size_t frames_per_pict = 5;
	unsigned int frameNo = 0;
	int frameForAngle = 0;

	bool isReverse = false;
	int lastMouseInput = 0;
	int bgIdx = 0;
	while (ProcessMessage() != -1) {
		ClearDrawScreen();
		GetHitKeyStateAll(keystate.data());

		//背景
		DrawExtendGraph(0, 0, 640, 480, bgH[bgIdx / 8], false);
		bgIdx = (bgIdx + 1) % 32;

		auto currentMouseInput = GetMouseInput();
		if ((currentMouseInput & MOUSE_INPUT_LEFT) &&
			!(lastMouseInput & MOUSE_INPUT_LEFT)) {
			isReverse = !isReverse;
		}
		lastMouseInput = currentMouseInput;





		int imageWidth, imageHeight;
		GetGraphSize(arrowH, &imageWidth, &imageHeight);
		constexpr size_t block_size = 16;
		constexpr size_t width = 1000;
		auto count = width / block_size;
		float weight = (float)imageWidth / (float)width;
		constexpr int base_y = 240;
		constexpr float sin_amp = block_size * 2;
		int x = 0;
		int y = base_y + sin_amp *
			sin(DegreeToRadian((float)(frameForAngle)));
		Position2 currentPos(x, y);
		//Vector2 lastDelta90Vec = Vector2::Zero();//直前のベクトル
		Position2 lastDelta90Vectors[2] = { Vector2::ZERO,Vector2::ZERO };
		Position2 lastPos = { 0.0f,0.0f };
		for (int i = 1; i <= count; ++i) {
			int nextX = i * block_size;
			int nextY = sin_amp *
				sin(DegreeToRadian((float)(0.5f * nextX + frameForAngle)));

			auto deltaVec = Vector2(block_size, nextY).Normalized() * block_size;//ちょっとだけべくとる
			auto nextPos = currentPos + deltaVec;

			auto middleVec0 = deltaVec;
			auto middleVecR = deltaVec.Rotated90();
			if (!(lastDelta90Vectors[0] == Vector2::ZERO)) {
				middleVecR = (middleVecR + lastDelta90Vectors[0]).Normalized() * block_size;
			}
			auto middleVecL = lastDelta90Vectors[0];

			if (!(lastDelta90Vectors[1] == Vector2::ZERO)) {
				middleVecL = (middleVecL + lastDelta90Vectors[1]).Normalized() * block_size;
			}
			lastDelta90Vectors[1] = lastDelta90Vectors[0];
			lastDelta90Vectors[0] = deltaVec.Rotated90();
			//if (!(lastDeltaVectors[0] == Vector2(0.0f, 0.0f))) {
			//	middleVec0 = (deltaVec.Rotated90() + lastDeltaVectors[0]).Normalized() * block_size;
			//}
			//auto middleVec1 = deltaVec;
			//if (!(lastDeltaVectors[1] == Vector2(0.0f, 0.0f))) {
			//	middleVec1 = (lastDeltaVectors[0]+ lastDeltaVectors[1]).Normalized() * block_size;
			//}

			//////四角形表示
			//DrawLineAA(//上辺
			//	currentPos.x, currentPos.y, //始点
			//	nextPos.x, nextPos.y, //終点
			//	0xffffff, 10.0f);

			auto rightPos = nextPos + deltaVec.Rotated90();
			//DrawLineAA(//右辺
			//	nextPos.x, nextPos.y, //始点
			//	rightPos.x, rightPos.y, //終点
			//	0xffffff, 2.0f);

			auto leftPos = currentPos + deltaVec.Rotated90();
			//DrawLineAA(//左辺
			//	currentPos.x, currentPos.y, //始点
			//	leftPos.x, leftPos.y, //終点
			//	0x8888ff, 2.0f);

			//DrawRectModiGraph(
			//	currentPos.x, currentPos.y, //左上
			//	nextPos.x, nextPos.y, //右上
			//	rightPos.x, rightPos.y , //右下
			//	leftPos.x, leftPos.y, //左下
			//	48,0,//元画像の左上
			//	16,16,//元画像の切り抜き幅、切り抜き高さ
			//	bgAssetH,true);


			//auto middlePosL = currentPos + middleVec1;
			//auto middlePosR = nextPos + middleVec0;

			if (lastPos == Vector2(0.0f, 0.0f)) {
				auto middlePosL = currentPos + middleVecL * 2;
				auto middlePosR = nextPos + middleVecR * 2;
				DrawRectModiGraph(
					currentPos.x, currentPos.y, //左上
					nextPos.x, nextPos.y, //右上
					middlePosR.x, middlePosR.y, //右下
					middlePosL.x, middlePosL.y, //左下
					i * block_size, 0,//元画像の左上
					block_size, 64,//元画像の切り抜き幅、切り抜き高さ
					arrowH, true);
			}
			else {
				auto middlePosL = lastPos + middleVecL * 2;
				auto middlePosR = currentPos + middleVecR * 2;
				DrawRectModiGraph(
					lastPos.x, lastPos.y, //左上
					currentPos.x, currentPos.y, //右上
					middlePosR.x, middlePosR.y, //右下
					middlePosL.x, middlePosL.y, //左下
					i * block_size * weight, 0,//元画像の左上
					block_size, 64,//元画像の切り抜き幅、切り抜き高さ
					arrowH, true);
			}

			//DrawLineAA(//右辺
			//	currentPos.x, currentPos.y, //始点
			//	middlePosR.x, middlePosR.y, //終点
			//	0xffffff, 2.0f);
			//DrawLineAA(//左辺
			//	lastPos.x, lastPos.y, //始点
			//	middlePosL.x, middlePosL.y, //終点
			//	0xffffff, 2.0f);

			//DrawLineAA(//中間
			//	currentPos.x, currentPos.y, //始点
			//	middlePos.x, middlePos.y, //終点
			//	0xff8888, 5.0f);

			//lastDeltaVectors[1] = lastDeltaVectors[0];
			//lastDeltaVectors[0] = deltaVec.Rotated90();//直前のを保存


			//DrawRectModiGraph(
			//	currentPos.x, currentPos.y, //左上
			//	nextPos.x, nextPos.y, //右上
			//	middlePosR.x, middlePosR.y , //右下
			//	middlePosL.x, middlePosL.y, //左下
			//	48,0,//元画像の左上
			//	16,16,//元画像の切り抜き幅、切り抜き高さ
			//	bgAssetH,true);


			//DrawLineAA(
			//	nextX, nextY + block_size,//始点
			//	x, y+ block_size, //終点
			//	0xffffff, 2.0f);
			//DrawLineAA(
			//	x, y + block_size, //始点
			//	x, y, //終点
			//	0xffffff, 2.0f);

			lastPos = currentPos;
			currentPos = nextPos;
		}
		//DrawModiGraph(x, y, //左上
		//	nextX, nextY,//右上
		//	nextX,nextY+32,//右下(右上から真下に落としてる)
		//	x,y+32, //左下(左上から真下に落としている)
		//	groundH,true);
		//DrawRectModiGraph(x, y, //左上
		//	nextX, nextY,//右上
		//	nextX,nextY+32,//右下(右上から真下に落としてる)
		//	x,y+32, //左下(左上から真下に落としている)
		//	48,0,//切り抜き元画像の左上
		//	16, 16,//元画像の幅、高さ
		//	bgAssetH,true);



		MoveByInput(keystate, rcA.center);
		DrawBox(rcA.Left(), rcA.Top(), rcA.Right(), rcA.Bottom(), 0xffffff, true);
		//wchar_t out[64];
		//wsprintf(out, L"x=%d , y=%d\n", (int)rcA.center.x, (int)rcA.center.y);
		//OutputDebugString(out);

		std::wostringstream oss;
		oss << L"x=" << rcA.center.x;
		oss << L" , y=" << rcA.center.y << std::endl;
		OutputDebugString(oss.str().c_str());

		//現在のマウス座標を取得
		auto mpos = GetMousePosition2();
		Vector2 dir = mpos - rcA.center;

		//基準(0°)が右ベクトルで、マウスの向きを角度で返す
		float angle = atan2(dir.y, dir.x);


		auto imgIdx = frameNo / frames_per_pict;
		int centerX = 16;
		if (isReverse) {
			int w, h;
			GetGraphSize(graphH[imgIdx], &w, &h);
			centerX = w - centerX;
		}
		DrawRotaGraph2(
			rcA.center.x, rcA.center.y,//画像の表示位置
			centerX, 35,//上の画像表示位置に、画像じたいのこの座標が合うように表示
			4.0f, angle, graphH[imgIdx], true, isReverse);


		DrawCircleAA(rcA.center.x, rcA.center.y, 5.0f, 16, 0xff0000);
		//sprintf_s(out, sizeof(out),"x=%3.2f , y=%3.2f\n",rcA.center.x,rcA.center.y);
		//OutputDebugStringA(out);

		Debug::Out(16);
		//Debug::Out(3.14f);
		//Debug::Out("Debug Out\n");
		//Debug::Out(L"Debug WOut\n");

		ScreenFlip();
		frameNo = (frameNo + 1) % (frames_per_pict * picture_num);
		frameForAngle = (frameForAngle + 1) % 720;
	}
	Vector2 v = { 10,10 };
	auto vv = rcA.center + v;

	test++;
	singleton.OutputLife();
	singleton.OutputLife();
	singleton.OutputLife();
	singleton.OutputLife();

	singleton2.OutputLife();
	DxLib_End();
	return 0;
}