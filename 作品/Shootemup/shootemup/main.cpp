#include<DxLib.h>
#include<cmath>
#include"Geometry.h"
#include <time.h>

///�����蔻��֐�
///@param posA A�̍��W
///@param radiusA A�̔��a
///@param posB B�̍��W
///@param radiusB B�̔��a
bool IsHit(const Circle& cA, const Circle& cB) {
	//�����蔻����������Ă�������
	auto totalRadius{ cA.radius + cB.radius };
	return (cA.pos - cB.pos).SQMagnitude() <= totalRadius * totalRadius;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	SetMainWindowText("2016010_�������l");
	if (DxLib_Init() != 0) {
		return -1;
	}
	SetFontSize(24);
	srand((unsigned int)time(NULL));
	SetDrawScreen(DX_SCREEN_BACK);

	//�w�i�p
	int bgH[4];
	LoadDivGraph("img/bganim.png", 4, 4, 1, 256, 192, bgH);

	int skyH = LoadGraph("img/sky.png");
	int sky2H = LoadGraph("img/sky2.png");

	auto bulletH=LoadGraph("img/bullet.png");
	int playerH[10];
	LoadDivGraph("img/player.png", 10, 5, 2, 16, 24,playerH );

	int enemyH[2];
	LoadDivGraph("img/enemy.png", 2, 2, 1, 32, 32, enemyH);

	constexpr float bullet_radius = 5.0f;
	constexpr float player_radius = 10.0f;
	struct Bullet {
		Circle circle;
		Vector2 vel;//���x
		bool isActive = false;//�����Ă邩�`�H
	};

	//�K����1024���炢����Ƃ�
	Bullet bullets[1024];

	Position2 enemypos(320,25);//�G���W
	Circle player(Position2(320, 400), player_radius);//���@���W

	unsigned int frame = 0;//�t���[���Ǘ��p

	char keystate[256];
	bool isDebugMode = false;
	int skyy = 0;
	int skyy2 = 0;
	int bgidx = 0;

	constexpr int frame_for_bullet_type = 180;
	constexpr int bullet_type_num = 7;

	while (ProcessMessage() == 0) {
		ClearDrawScreen();

		GetHitKeyStateAll(keystate);

		isDebugMode = keystate[KEY_INPUT_P];

		//�w�i
		DrawExtendGraph(0, 0, 640, 480, bgH[bgidx / 8], false);
		bgidx = (bgidx + 1) % 32;

		//SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
		skyy = (skyy + 1) % 480;
		skyy2 = (skyy2 + 2) % 480;
		DrawExtendGraph(0, skyy, 640, skyy + 480, skyH, true);
		DrawExtendGraph(0, skyy - 480, 640, skyy, skyH, true);
		DrawExtendGraph(0, skyy2, 640, skyy2 + 480, sky2H, true);
		DrawExtendGraph(0, skyy2 - 480, 640, skyy2, sky2H, true);


		//�v���C���[
		if (keystate[KEY_INPUT_RIGHT]) {
			player.pos.x = min(640,player.pos.x+4);
		}
		else if (keystate[KEY_INPUT_LEFT]) {
			player.pos.x = max(0,player.pos.x-4);
		}
		if (keystate[KEY_INPUT_UP]) {
			player.pos.y = max(0,player.pos.y-4);
		}else if (keystate[KEY_INPUT_DOWN]) {
			player.pos.y = min(480,player.pos.y+4);
		}

		int pidx = (frame/4 % 2)*5+3;
		DrawRotaGraph(player.pos.x, player.pos.y, 2.0f, 0.0f, playerH[pidx], true);
		if (isDebugMode) {
			//���@�̖{��(�����蔻��)
			DrawCircle(player.pos.x, player.pos.y, player_radius, 0xffaaaa, false, 3);
		}

		constexpr float base_speed = 5.0f;
		int blazing_period = 12;
		if (frame < frame_for_bullet_type) {
			DrawString(10, 10, "����", 0xffffff);
			//�e����
			if (frame % blazing_period == 0) {
				for (auto& b : bullets) {
					if (!b.isActive) {
						b.circle.pos = enemypos;// ���W�͓G�@
						b.vel = Vector2(0, base_speed);// �܂��͐^���ɔ�΂�
						b.isActive = true;
						break;
					}
				}
			}
		}
		else if(frame<frame_for_bullet_type * 2){// ���@�_��
			DrawString(10, 10, "���@�_��", 0xffffff);
			//�e����
			if (frame % blazing_period == 0) {
				for (auto& b : bullets) {
					if (!b.isActive) {
						b.circle.pos = enemypos;// ���W�͓G�@
						b.vel = (player.pos - enemypos).Normalized() * base_speed;
						b.isActive = true;
						break;
					}
				}
			}
		}
		else if (frame < frame_for_bullet_type * 3){// 3WayShot
			DrawString(10, 10, "3Way�e", 0xffffff);
			//�e����
			if (frame % blazing_period == 0) {
				int count = 0;
				auto dir = (player.pos - enemypos);
				float bangle = atan2(dir.y, dir.x);
				bangle -= DX_PI_F / 6.0f;
				for (auto& b : bullets) {
					if (!b.isActive) {
						b.circle.pos = enemypos;// ���W�͓G�@
						b.vel = Vector2(cos(bangle), sin(bangle)) * base_speed;
						b.isActive = true;
						bangle += DX_PI_F / 6.0f;
						++count;
					}
					if (count == 3) {
						break;
					}
				}
			}
		}
		else if (frame < frame_for_bullet_type * 4) {// ���ˏ�e
			DrawString(10, 10, "���ˏ�e", 0xffffff);
			//�e����
			if (frame % blazing_period == 0) {
				int count = 0;
				auto dir = Vector2{ 10,10 };
				float bangle = atan2(dir.y, dir.x);
				bangle -= DX_PI_F / 12.0f;
				for (auto& b : bullets) {
					if (!b.isActive) {
						b.circle.pos = enemypos;// ���W�͓G�@
						b.vel = Vector2(cos(bangle), sin(bangle)) * base_speed;
						b.isActive = true;
						bangle += DX_PI_F / 12.0f;
						++count;
					}
					if (count == 24) {
						break;
					}
				}
			}
		}
		else if (frame < frame_for_bullet_type * 5) {// �΂�܂�
			DrawString(10, 10, "�΂�܂��e", 0xffffff);
			//�e����
			if (frame % blazing_period == 0) {
				int count = 0;
				auto dir = Vector2{ 10,10 };
				float bangle = atan2(dir.y, dir.x);
				bangle -= DX_PI_F / 12.0f;
				for (auto& b : bullets) {
					if (!b.isActive) {
						b.circle.pos = enemypos;// ���W�͓G�@
						b.vel = Vector2(cos(bangle), sin(bangle)) * (base_speed + (rand() % 3));
						b.isActive = true;
						bangle += DX_PI_F / 12.0f;
						++count;
					}
					if (count == 24) {
						break;
					}
				}
			}
		}
		else if (frame < frame_for_bullet_type * 6) {// ���@�_������
			DrawString(10, 10, "���@�_���Q", 0xffffff);
			blazing_period -= 11;
			//�e����
			if (frame % blazing_period == 0) {
				for (auto& b : bullets) {
					if (!b.isActive) {
						b.circle.pos = enemypos;// ���W�͓G�@
						b.vel = (player.pos - enemypos).Normalized() * base_speed;
						b.isActive = true;
						break;
					}
				}
			}
		}
		else if (frame < frame_for_bullet_type * 7) {// �I���W�i��
			DrawString(10, 10, "�I���W�i��", 0xffffff);
			//�e����
			if (frame % blazing_period == 0) {
				int count = 0;
				auto dir = (player.pos);
				float bangle = atan2(dir.y, dir.x);
				bangle -= DX_PI_F / 3.0f;
				for (auto& b : bullets) {
					if (!b.isActive) {
						b.circle.pos = Vector2{0,0};// ���W�͓G�@
						b.vel = Vector2(cos(bangle), sin(bangle)) * base_speed * 2;
						b.isActive = true;
						bangle += DX_PI_F / 12.0f;
						++count;
					}
					if (count == 6) {
						break;
					}
				}
			}
			if (frame % blazing_period == 0) {
				int count = 0;
				auto dir = (player.pos);
				float bangle = atan2(dir.y, dir.x);
				//bangle += DX_PI_F / 3.0f;
				for (auto& b : bullets) {
					if (!b.isActive) {
						b.circle.pos = Vector2{ 640,0 };// ���W�͓G�@
						b.vel = Vector2(cos(bangle), sin(bangle)) * base_speed * 2;
						b.isActive = true;
						bangle += DX_PI_F / 12.0f;
						++count;
					}
					if (count == 6) {
						break;
					}
				}
			}
		}

		//�e�̍X�V����ѕ\��
		for (auto& b : bullets) {
			if (!b.isActive) {
				continue;
			}

			//�e�̌��ݍ��W�ɒe�̌��ݑ��x�����Z���Ă�������
			b.circle.pos += b.vel;
			
			//�e�̊p�x��atan2�Ōv�Z���Ă��������Bangle�ɒl������񂾂�I�D
			float angle = atan2(b.vel.y, b.vel.x);
			
			DrawRotaGraph(b.circle.pos.x, b.circle.pos.y,1.0f,angle, bulletH, true);
			
			if (isDebugMode) {
				//�e�̖{��(�����蔻��)
				DrawCircle(b.circle.pos.x, b.circle.pos.y, bullet_radius, 0x0000ff, false, 3);
			}
			//�e���E��
			if (b.circle.pos.x + 16 < 0 || b.circle.pos.x - 16 > 640 ||
				b.circle.pos.y + 24 < 0 || b.circle.pos.y - 24 > 480) {
				b.isActive = false;
			}

			//������I
			//����IsHit�͎����������Ă܂���B�����ŏ����Ă��������B
			if (b.isActive && IsHit(b.circle, player)) {
				//�������������������Ă��������B
				//�����������DrawString���Ă��������A���������Ă������B�G�̋ʂ������܂��傤
				DrawString(player.pos.x - 40, player.pos.y - 24, "������", 0xffffff);
				b.isActive = false;
			}
		}

		//�G�̕\��
		enemypos.x = abs((int)((frame + 320) % 1280) - 640);
		int eidx = (frame / 4 % 2);
		DrawRotaGraph(enemypos.x, enemypos.y, 2.0f, 0.0f, enemyH[eidx],true);

		if (isDebugMode) {
			//�G�̖{��(�����蔻��)
			DrawCircle(enemypos.x, enemypos.y, 5, 0xffffff, false, 3);
		}
		frame = (frame + 1) % (frame_for_bullet_type * bullet_type_num);
		ScreenFlip();
	}

	DxLib_End();

	return 0;
}