#include<DxLib.h>
#include<array>//stl�Ŕz���\������
#include<cassert>
#include<memory>//�X�}�[�g�|�C���^
#include<sstream>//������X�g���[��
#include<iomanip>//�X�g���[���}�j�s�����[�^
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
/// �x���@���ʓx�@�ɕϊ�
/// </summary>
/// <param name="deg">�x��</param>
/// <returns>���W�A��</returns>
float DegreeToRadian(float deg) {
	return deg * DX_PI / 180.0f;
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	Singleton& singleton = Singleton::Instance();
	//���R�s�[�R���X�g���N�^���L���ɂȂ��Ă�
	auto& singleton2 = Singleton::Instance();



	SetWindowText(L"2016010_�������l");
	ChangeWindowMode(true);
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);

	//�w�i�p
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

		//�w�i
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
		//Vector2 lastDelta90Vec = Vector2::Zero();//���O�̃x�N�g��
		Position2 lastDelta90Vectors[2] = { Vector2::ZERO,Vector2::ZERO };
		Position2 lastPos = { 0.0f,0.0f };
		for (int i = 1; i <= count; ++i) {
			int nextX = i * block_size;
			int nextY = sin_amp *
				sin(DegreeToRadian((float)(0.5f * nextX + frameForAngle)));

			auto deltaVec = Vector2(block_size, nextY).Normalized() * block_size;//������Ƃ����ׂ��Ƃ�
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

			//////�l�p�`�\��
			//DrawLineAA(//���
			//	currentPos.x, currentPos.y, //�n�_
			//	nextPos.x, nextPos.y, //�I�_
			//	0xffffff, 10.0f);

			auto rightPos = nextPos + deltaVec.Rotated90();
			//DrawLineAA(//�E��
			//	nextPos.x, nextPos.y, //�n�_
			//	rightPos.x, rightPos.y, //�I�_
			//	0xffffff, 2.0f);

			auto leftPos = currentPos + deltaVec.Rotated90();
			//DrawLineAA(//����
			//	currentPos.x, currentPos.y, //�n�_
			//	leftPos.x, leftPos.y, //�I�_
			//	0x8888ff, 2.0f);

			//DrawRectModiGraph(
			//	currentPos.x, currentPos.y, //����
			//	nextPos.x, nextPos.y, //�E��
			//	rightPos.x, rightPos.y , //�E��
			//	leftPos.x, leftPos.y, //����
			//	48,0,//���摜�̍���
			//	16,16,//���摜�̐؂蔲�����A�؂蔲������
			//	bgAssetH,true);


			//auto middlePosL = currentPos + middleVec1;
			//auto middlePosR = nextPos + middleVec0;

			if (lastPos == Vector2(0.0f, 0.0f)) {
				auto middlePosL = currentPos + middleVecL * 2;
				auto middlePosR = nextPos + middleVecR * 2;
				DrawRectModiGraph(
					currentPos.x, currentPos.y, //����
					nextPos.x, nextPos.y, //�E��
					middlePosR.x, middlePosR.y, //�E��
					middlePosL.x, middlePosL.y, //����
					i * block_size, 0,//���摜�̍���
					block_size, 64,//���摜�̐؂蔲�����A�؂蔲������
					arrowH, true);
			}
			else {
				auto middlePosL = lastPos + middleVecL * 2;
				auto middlePosR = currentPos + middleVecR * 2;
				DrawRectModiGraph(
					lastPos.x, lastPos.y, //����
					currentPos.x, currentPos.y, //�E��
					middlePosR.x, middlePosR.y, //�E��
					middlePosL.x, middlePosL.y, //����
					i * block_size * weight, 0,//���摜�̍���
					block_size, 64,//���摜�̐؂蔲�����A�؂蔲������
					arrowH, true);
			}

			//DrawLineAA(//�E��
			//	currentPos.x, currentPos.y, //�n�_
			//	middlePosR.x, middlePosR.y, //�I�_
			//	0xffffff, 2.0f);
			//DrawLineAA(//����
			//	lastPos.x, lastPos.y, //�n�_
			//	middlePosL.x, middlePosL.y, //�I�_
			//	0xffffff, 2.0f);

			//DrawLineAA(//����
			//	currentPos.x, currentPos.y, //�n�_
			//	middlePos.x, middlePos.y, //�I�_
			//	0xff8888, 5.0f);

			//lastDeltaVectors[1] = lastDeltaVectors[0];
			//lastDeltaVectors[0] = deltaVec.Rotated90();//���O�̂�ۑ�


			//DrawRectModiGraph(
			//	currentPos.x, currentPos.y, //����
			//	nextPos.x, nextPos.y, //�E��
			//	middlePosR.x, middlePosR.y , //�E��
			//	middlePosL.x, middlePosL.y, //����
			//	48,0,//���摜�̍���
			//	16,16,//���摜�̐؂蔲�����A�؂蔲������
			//	bgAssetH,true);


			//DrawLineAA(
			//	nextX, nextY + block_size,//�n�_
			//	x, y+ block_size, //�I�_
			//	0xffffff, 2.0f);
			//DrawLineAA(
			//	x, y + block_size, //�n�_
			//	x, y, //�I�_
			//	0xffffff, 2.0f);

			lastPos = currentPos;
			currentPos = nextPos;
		}
		//DrawModiGraph(x, y, //����
		//	nextX, nextY,//�E��
		//	nextX,nextY+32,//�E��(�E�ォ��^���ɗ��Ƃ��Ă�)
		//	x,y+32, //����(���ォ��^���ɗ��Ƃ��Ă���)
		//	groundH,true);
		//DrawRectModiGraph(x, y, //����
		//	nextX, nextY,//�E��
		//	nextX,nextY+32,//�E��(�E�ォ��^���ɗ��Ƃ��Ă�)
		//	x,y+32, //����(���ォ��^���ɗ��Ƃ��Ă���)
		//	48,0,//�؂蔲�����摜�̍���
		//	16, 16,//���摜�̕��A����
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

		//���݂̃}�E�X���W���擾
		auto mpos = GetMousePosition2();
		Vector2 dir = mpos - rcA.center;

		//�(0��)���E�x�N�g���ŁA�}�E�X�̌������p�x�ŕԂ�
		float angle = atan2(dir.y, dir.x);


		auto imgIdx = frameNo / frames_per_pict;
		int centerX = 16;
		if (isReverse) {
			int w, h;
			GetGraphSize(graphH[imgIdx], &w, &h);
			centerX = w - centerX;
		}
		DrawRotaGraph2(
			rcA.center.x, rcA.center.y,//�摜�̕\���ʒu
			centerX, 35,//��̉摜�\���ʒu�ɁA�摜�������̂��̍��W�������悤�ɕ\��
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