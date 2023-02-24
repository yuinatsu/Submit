#include<DxLib.h>
#include<cmath>
#include<random>
#include"Geometry.h"
#include <time.h>

#define PI	3.1415926535897932384626433832795f

using namespace std;

struct Rock {
	Circle circle;
	float angle;
	float speed;
	bool alive;
};

void DrawWood(const Capsule& cap, int handle) {
	auto v = cap.posB - cap.posA;
	auto angle = atan2(v.y, v.x) + DX_PI_F / 2.0f;
	auto w = cos(angle)*cap.radius;
	auto h = sin(angle)*cap.radius;

	DrawModiGraph(
		cap.posA.x - w, cap.posA.y - h,
		cap.posB.x - w, cap.posB.y - h,
		cap.posB.x + w, cap.posB.y + h,
		cap.posA.x + w, cap.posA.y + h, handle,true);
}


///�Ƃ���_�𒆐S�ɉ�]���āA���̉�]��̍��W��Ԃ��܂��B
///@param center ��]���S�_
///@param angle ��]�p�x
///@param pos ���̍��W
///@return �ϊ���̍��W
Matrix RotatePosition(const Position2& center, float angle) {
	//�@���S�����_�ɕ��s�ړ�����
	//�A���_���S�ɉ�]����
	//�B���S�����̍��W�֖߂�

	Matrix mat = IdentityMat();
	mat = MultipleMat(TranslateMat(center.x, center.y),
		MultipleMat(RotateMat(angle), TranslateMat(-center.x, -center.y)));
	return mat;
	//��������������āA����̓_�𒆐S�ɉ�]���s���悤�ɂ��Ă��������B
}

float Clamp(float vlate, float maxval = 1.0f, float minval = 0.0f) {
	return min(max(vlate, minval), maxval);
}

//�J�v�Z���Ɖ~�������������H
bool IsHit(const Capsule& cap, const Circle& cc) {
	auto vp = cap.posA - cc.pos;
	auto v = cap.posA - cap.posB;
	auto dot = Dot(vp, v);
	auto vlate = dot / (v.Magnitude() * v.Magnitude());
	vlate = Clamp(vlate);
	v *= vlate;
	auto vq = vp - v;
	return (vq.Magnitude() <= (cap.radius + cc.radius));
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	SetGraphMode(512, 800, 32);
	SetMainWindowText("2016010_�������l");
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);

	srand((unsigned int)time(NULL));

	int sw, sh;//�X�N���[�����A����
	GetDrawScreenSize(&sw, &sh);

	auto woodH = LoadGraph("img/wood.png");
	int wdW, wdH;
	GetGraphSize(woodH, &wdW, &wdH);
	wdW = 200;

	auto cascadeH = LoadGraph("img/cascade_chip.png");
	auto chipH = LoadGraph("img/atlas0.png");
	auto rockH = LoadGraph("img/rock.png");

 	Capsule cap(20,Position2((sw-wdW)/2,sh-100),Position2((sw - wdW) / 2+wdW,sh-100));
	bool plAlive = true;
	int playerLH = LoadGraph("img/mario.png");
	int playerRH = LoadGraph("img/luigi.png");
	float plFallSpeed = 0.2f;

	Rock rocks[5];
	for (int i = 0; i < 5; i++)
	{
		rocks[i].alive = false;
		rocks[i].circle.radius = 12.0f;
	}
	std::mt19937 mt;
	std::uniform_real_distribution<float> rockPosRange(42.0f, static_cast<float>(sw - 42));
	std::uniform_real_distribution<float> rockSpeedRange(1.0f, 3.0f);
	
	int blastH[12];
	LoadDivGraph("img/explosion.png", 12, 12, 1, 128, 128, blastH);
	int blastCnt = 0;
	int blastFlag = false;
	Vector2 blastPos = { 0.0f, 0.0f };

	bool rotFlag = false;

	char keystate[256];
	
	float angle = 0.0f;

	int frame = 0;
	bool isLeft = false;

	float wallL = 32.0f;
	float wallR = sw - 32.0f;

	int clearH = LoadGraph("img/clear.png");
	float clearLine = 32.0;
	bool clearFlag = false;


	while (ProcessMessage() == 0) {
		ClearDrawScreen();
		GetHitKeyStateAll(keystate);

		DrawBox(0, 0, sw, sh, 0xaaffff, true);

		if (frame %  200 == 0)
		{
			for (int i = 0; i < 5; i++)
			{
				if (!rocks[i].alive)
				{
					rocks[i].alive = true;
					rocks[i].circle.pos = { (float)(rand() % 512), 0.0f };
					rocks[i].speed = rand() % 4 + 1;
					//rocks[i].circle.pos = { rockPosRange(mt), 0.0f };
					//rocks[i].speed = rockSpeedRange(mt);
					rocks[i].angle = 0.0f;
					break;
				}
			}
		}

		for (int i = 0; i < 5; i++)
		{
			if (rocks[i].alive)
			{
				rocks[i].circle.pos.y += rocks[i].speed;
				rocks[i].angle += 0.01f;
			}
		}

		int mx = 0, my = 0;

		if (clearFlag)
		{
			if (keystate[KEY_INPUT_SPACE])
			{
				clearFlag = false;
				angle = 0.0f;
				cap.posA = { static_cast<float>((sw - wdW) / 2), static_cast<float>(sh - 100) };
				cap.posB = { static_cast<float>((sw - wdW) / 2 + wdW), static_cast<float>(sh - 100) };
			}
		}
		else
		{
			// �ر����
			if ((cap.posA.y < clearLine) && (cap.posB.y < clearLine))
			{
				clearFlag = true;		
				for (int i = 0; i < 5; i++)
				{
					rocks[i].alive = false;
				}
			}

			if (plAlive)
			{
				cap.posA.y += plFallSpeed;
				cap.posB.y += plFallSpeed;

				if (keystate[KEY_INPUT_LEFT]) {
					isLeft = true;
				}
				else if (keystate[KEY_INPUT_RIGHT]) {
					isLeft = false;
				}
				if (isLeft) {
					mx = cap.posA.x;
					my = cap.posA.y;
				}
				else {
					mx = cap.posB.x;
					my = cap.posB.y;
				}
				if (keystate[KEY_INPUT_Z]) {

					angle = -0.05f;
				}
				else if (keystate[KEY_INPUT_X]) {

					angle = 0.05f;
				}
				else {
					angle = 0.0f;
				}

				// ���S����������ɏ����Ă���
				// �����蔻������������ē��������Ƃ��̔����������Ă�������
				for (int i = 0; i < 5; i++)
				{
					if (rocks[i].alive)
					{
						if (IsHit(cap, rocks[i].circle)) {
							rocks[i].alive = false;
							plAlive = false;
							blastFlag = true;
							blastPos = { (cap.posA.x + cap.posB.x) / 2, (cap.posA.y + cap.posB.y) / 2 };
						}
					}
				}
				// 180�x�ȏ�
				if (cap.posB.x < cap.posA.x)
				{
					plAlive = false;
					rotFlag = true;
				}
				// �ǂɓ�����
				if (cap.posA.x < wallL ||
					cap.posB.x > wallR ||
					(cap.posA.y > sh && cap.posB.y > sh))
				{
					plAlive = false;
				}
				

				//�J�v�Z����]
				Matrix rotMat = RotatePosition(Position2(mx, my), angle);
				cap.posA = MultipleVec(rotMat, cap.posA);
				cap.posB = MultipleVec(rotMat, cap.posB);
			}
		}

		//�w�i�̕`��
		//��
		int chipIdx = (frame/4) % 3;
		constexpr int dest_chip_size = 32;
		int destY = 48;
		while (destY < sh) {
			int destX = 0;
			while (destX < sw) {
				DrawRectExtendGraph(
					destX, destY, destX + dest_chip_size, destY+dest_chip_size,
					chipIdx * 16, 0,
					16, 16,
					cascadeH, false);
				destX += dest_chip_size;
			}
			destY += dest_chip_size;
		}

		DrawRectGraph(0, 0, 96, 0, 32, 32, chipH, true);
		DrawRectGraph(sw - 32, 0, 32, 0, 32, 32, chipH, true);
		DrawRectGraph(0, 32, 96, 32, 32, 32, chipH, true);
		DrawRectGraph(sw-32, 32, 32, 32, 32, 32, chipH, true);
		destY = 64;
		while (destY < sh) {
			DrawRectGraph(0, destY, 96, 64, 32, 32, chipH, true);
			DrawRectGraph(sw - 32, destY, 32, 64, 32, 32, chipH, true);
			destY += dest_chip_size;
		}

		// ��ڲ԰�̕`��Ǝ��S���o�Ǝ��S����
		if (plAlive)
		{
			DrawWood(cap, woodH);
			if(!clearFlag)
			{
				DrawCircle(mx, my, 30, 0xff0000, false, 3);
			}
		}
		else
		{
			// ��������
			if (blastFlag)
			{
				DrawRotaGraph(blastPos.x, blastPos.y, 1.0, 0.0, blastH[blastCnt / 2], true);
				blastCnt++;
				if (blastCnt >= 24)
				{
					blastFlag = false;
					blastCnt = 0;
				}
			}
			DrawRotaGraph(cap.posA.x, cap.posA.y, 1.8, PI, playerLH, true);
			DrawRotaGraph(cap.posB.x, cap.posB.y, 1.8, PI, playerRH, true);
			cap.posA.y += plFallSpeed;
			cap.posB.y += plFallSpeed;
			plFallSpeed += 0.5f;
			if ((cap.posA.y + 30.0f > sh) && (cap.posB.y + 30.0f > sh))
			{
				plAlive = true;
				angle = 0.0f;
				cap.posA = { static_cast<float>((sw - wdW) / 2), static_cast<float>(sh - 100) };
				cap.posB = { static_cast<float>((sw - wdW) / 2 + wdW), static_cast<float>(sh - 100) };
				plFallSpeed = 0.2f;
			}
		}

		for (int i = 0; i < 5; i++)
		{
			if (rocks[i].alive)
			{
				DrawRotaGraph(rocks[i].circle.pos.x, rocks[i].circle.pos.y + 24, 2.0, rocks[i].angle, rockH, true);
			}
		}

		if (clearFlag)
		{
			DrawBox(0, 0, sw, sh, 0xffffff, true);
			DrawRotaGraph(sw / 2, sh / 2, 1.0, 0.0, clearH, true);
			SetFontSize(32);
			if ((frame / 50) % 2)
			{
				DrawString(40, 480, "PUSH SPACE TO GAME START", 0x000000);
			}
		}

		++frame;
		
		ScreenFlip();
	}

	DxLib_End();


}