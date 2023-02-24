#pragma once

// �萔��`
//-----------------
#define SCREEN_SIZE_X 800			// ��ʂ̉��T�C�Y
#define SCREEN_SIZE_Y 600			// ��ʂ̏c�T�C�Y

#define GAME_SCREEN_X 40			// �Q�[����ʂ̕`��I�t�Z�b�g
#define GAME_SCREEN_Y 75			// �Q�[����ʂ̕`��I�t�Z�b�g
#define GAME_SCREEN_SIZE_X 640		// �Q�[����ʂ̉��T�C�Y
#define GAME_SCREEN_SIZE_Y 480		// �Q�[����ʂ̏c�T�C�Y

#define GAME_FONT_SIZE 18			// �t�H���g�̃T�C�Y

#define TITLE_SIZE_X 423			// �^�C�g���̉��T�C�Y
#define TITLE_SIZE_Y 198			// �^�C�g���̏c�T�C�Y

#define GAMEOVER_SIZE_X 452			// �Q�[���I�[�o�[���S�̉��T�C�Y
#define GAMEOVER_SIZE_Y 123			// �Q�[���I�[�o�[���S�̏c�T�C�Y

#define ENEMY_POINT 100				// �G�̓_��

#define PLAYER_SIZE_X 32			// ���@�̉��T�C�Y
#define PLAYER_SIZE_Y 32			// ���@�̏c�T�C�Y

#define PLAYER_SHOT_SIZE_X 6		// ���@�̒e�̉��T�C�Y
#define PLAYER_SHOT_SIZE_Y 30		// ���@�̒e�̏c�T�C�Y

#define ANIM_MAX 3					// �A�j���[�V�����̃R�}��

#define ENEMY_SIZE_X 32				// �G�̉��T�C�Y
#define ENEMY_SIZE_Y 32				// �G�̏c�T�C�Y

#define ENEMY_SHOT_SIZE_X 16		// �G�̒e�̉��T�C�Y
#define ENEMY_SHOT_SIZE_Y 25		// �G�̒e�̏c�T�C�Y

#define ENEMY_COUNT_X 8				// �G�̉����т̐�
#define ENEMY_COUNT_Y 7				// �G�̏c���т̐�
#define ENEMY_MOVE_INT 5			// �G�̈ړ��Ԋu

#define GET_ENEMY_POS_Y(y) (((y) * ENEMY_SIZE_Y) * 9 / 7)					// �G��Y���W

#define UI_DRAW_X (GAME_SCREEN_X + GAME_SCREEN_SIZE_X + 20)					// ��ʉE�̕\�����@X���W

// ����
enum DIR_ID {
	DIR_DOWN,			// ������
	DIR_LEFT,			// ������
	DIR_RIGHT,			// �E����
	DIR_UP,				// �����
	DIR_MAX
};

// �G���
enum ENEMY_TYPE {
	ENEMY_TYPE_1,
	ENEMY_TYPE_2,
	ENEMY_TYPE_3,
	ENEMY_TYPE_4,
	ENEMY_TYPE_5,
	ENEMY_TYPE_6,
	ENEMY_TYPE_UFO,
	ENEMY_TYPE_MAX
};

// �G�̍��E�ړ�����
enum MOVE_LR {
	MOVE_LEFT,			// ������
	MOVE_RIGHT,			// �E����
	MOVE_MAX
};

// �ړ����[�h�����E or ��
enum MOVE_MODE
{
	MOVE_MODE_LR,		// ���E�ړ����[�h
	MOVE_MODE_DOWN,		// ���ړ����[�h
	MOVE_MODE_MAX
};

// �V�[����ʑ���
enum SCENE_ID
{
	SCENE_ID_TITLE,
	SCENE_ID_GAME,
	SCENE_ID_GAMEOVER,
	SCENE_ID_RULE,
	SCENE_ID_MAX
};

// �v���g�^�C�v�錾
//-----------------
void TitleScene(void);					// �^�C�g�����
void TitleDraw(void);					// �^�C�g���`�揈��

void RuleScene(void);					// ���[�������p���
void RuleDraw(void);					// ���[�������\��

void GameScene(void);					// �Q�[�����
void PlayerCtl(void);					// ���@����
void PshotCtl(void);					// ���@�̒e����
void EnemyCtl(void);					// �G����
void CheckEnemyLine(void);				// �G�̖h�q���C�����B�m�F
bool DethProcPlayer(void);				// ���@�̎��S������
void CheckEnemyMoveLR(void);			// �G�����]���m�F		void...�Ԃ�l�Ȃ�
void EshotCtl(void);					// �G�̒e����
void CheckHitCtl(void);					// �����蔻�菈��
void CheckHitPshotEnemy(void);			// ���@�̒e�ƓG�̓����蔻�菈��
void CheckHitEshotPlayer(void);			// ���@�ƓG�̓����蔻�菈��
void ScrDraw(void);						// �X�R�AUI�\��
void DrawCount(int rows, const char* str, int Cnt, const char* lenStr);			// �X�R�A�\������
bool AddScr(int num);					// �X�R�A���Z����
void GameDraw(void);					// �Q�[���`�揈��

void GameOverScene(void);				// �Q�[���I�[�o�[���
void GameOverDraw(void);				// �Q�[���I�[�o�[�`�揈��

bool SysInit(void);						// �V�X�e���n������		�Ԃ�l��true��false
bool GameInit(void);					// �Q�[���V�X�e���n������