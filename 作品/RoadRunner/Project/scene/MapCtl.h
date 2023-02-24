#pragma once
#include "BaseScene.h"
#include "../common/Vector2.h"
#include "Obj/Obj.h"
#include "Obj/ChipID.h"
#include "Obj/ChipGP.h"
#include <vector>
#include <map>
#include <list>
#include <functional>			// �Ӗ��j������Ƃ������߁A����


#define lpMapCtl MapCtl::GetInstance()

using MapChipGP = std::map<ChipID, ChipGP>;

#define LDR_ID_NAME "LDR_MAP_DATA"
#define LDR_ID_VER 0x01					// �����̃o�[�W�����ԍ��͂O

#define DigCnt 10													// �P�R�}������̃t���[����
#define DigFrameLim 5												// �R�}��
#define DigWait (60 * 4)											// �S�b�Ԃ̑ҋ@����
#define DigFullFrame ((DigCnt * DigFrameLim) * 2 + DigWait)			// ���t���[����


struct DataHeader
{
// �f�t�H���g��public
	char fileID[13];			// �t�@�C����ID���						�Z�[�u�f�[�^�̕�����
	unsigned char verID;		// �t�@�C���̃o�[�W�����ԍ�				���g���o�[�W�����A�b�v���s������ID�𑝂₵�ČÂ��t�@�C���͎g���Ȃ��悤�ɂ���
	unsigned char res1[2];		// �A���C�����g���\��̈�Ƃ���			��������4�o�C�g�����ׂ邽�߂�2�o�C�g����		reserve ... �\��
	int sizeX;					// �}�b�v��X�T�C�Y
	int sizeY;					// �}�b�v��Y�T�C�Y
	unsigned char res2[3];		// �A���C�����g���\��̈�Ƃ���			��������4�o�C�g�����ׂ邽�߂�3�o�C�g����
	unsigned char sum;			// SUM�l
};


class MapCtl
{
public:
	static MapCtl& GetInstance(void)
	{
		return (*s_Instance);
	}
	static void Create(SceneID sceneID);
	static void Destroy(void);
	void DrawEditOffset();				// �}�b�v�O�I�t�Z�b�g�̕`��

	bool SetMapData(Vector2 pos, ChipID id);						// (���W, �`�b�v��ID)
	bool SetMapData(Vector2 pos, ChipID id, ChipGP gp);				// (���W, �`�b�v��ID, )
	ChipID GetMapDataEdit(ChipGP gpID, unsigned int data);				// (, baseMapID_)											�w��f�[�^����ChipID�����o��
	ChipID GetMapDataGame(ChipGP gpID, unsigned int data);
	ChipID GetMapData(ChipGP gpID, int x, int y, bool dig);			// (, �J�[�\����X���W�̃}�X��, �J�[�\����Y���W�̃}�X��, �@�邩�ǂ����̃`�F�b�N)		�w�肵���}�X�ڂ���f�[�^���擾���AChipID�����o��
	ChipID GetMapData(ChipGP gpID, Vector2 pos, bool dig);					// (, �J�[�\���̍��W)										�w�肵�����W����f�[�^���擾���AChipID�����o��

	bool SetDig(const Vector2& pos, DIR_LR dir);					// (���@���W, ���@�̌���)
	int GetDigData(int x, int y);									// 
	void DigUpdate(void);											// Dig���̍X�V

	bool EraceGold(Vector2 pos, bool playerFlag);					// Map������Gold������			(����̍��W, ���@�������Ă��邩�̃t���O)

	void InputButtonL(bool input);
	void InputButtonR(bool input);

	void SetCamera(std::weak_ptr<Camera> camera);
	int GetGoldCnt();							// ����̒l��Ԃ��ϐ�
	bool MapSave(void);							// �f�[�^�̕ۑ�		�g������ꍇ�͈�����num�����ĕۑ��ꏊ�w�肷��
	bool MapLoad(std::string fileName);							// �f�[�^�̓ǂݍ���				editScene�����gameScene�̓����ŌĂяo��
	bool MapLoad(std::string fileName, BaseScene& scene);				// gameScene
	bool MapDel(void);


	std::function<void(void)> DrawFunc;					// �֐��ۂ��Ǝ����Ă��Ċ֐������������΂���				�Ԃ�l�̌^�͍��킹��
	std::function<ChipID(ChipGP, unsigned int)> GetMapDataFunc;

	Vector2 GetDrawOffset();
private:
	bool CheckMapChar(ChipID id);			//�v���C���[�ݒu������
	void EditDraw(void);					// EditScene�p��Draw�֐�
	void GameDraw(void);					// GameScene�p��Draw�֐�
	void DrawMap(void);						// �}�b�v�ɕ\������}�b�v�`�b�v			GameDraw�y��EditDraw�֐��œ����I�ɌĂяo��
	void DrawEditLine(void);				// �}�b�v�ɕ\�����郉�C��			EditDraw�֐��œ����I�ɌĂяo��
	MapCtl(SceneID sceneID);				// MapCtl�̃C���X�g���N�^
	~MapCtl();								// MapCtl�̃f�X�g���N�^
	bool Init(void);						// MapCtl��Init
	bool InitMap(void);						// �}�b�v�n��Init
	bool InitDig(void);						// �@��n��Init
	void InitDrawChipEdit();				// EditScene��DrawChip_�̏�����
	void InitDrawChipGame();				// GameScene��DrawChip_�̏�����
	static MapCtl* s_Instance;				// MapCtl�̃C���X�^���X
	SceneID sceneID_;						// sceneID��ۑ����邽�߂̊�

	Vector2 viewArea_;						// �P��ʂɕ\�������}�b�v�̍L��
	Vector2 worldArea_;						// �}�b�v�̍L��
	Vector2 chipSize_;						// �`�b�v�̃T�C�Y
	Vector2 drawOffset_;					// �`��I�t�Z�b�g

	std::map<ChipID, std::function<void(const int x, const int y, const Vector2 & chipSize, const Vector2 & drawOffset, const int imageID)>> DrawChip_;


	// �Q�����z��
	//-------------
	std::vector<unsigned int> baseMapID_;
	std::vector<unsigned int*> mapID_;								// mapID_�̒��̃|�C���^���g�p����baseMapID_�ɃA�N�Z�X����		�|�C���^�̉ϐ�

	std::vector<unsigned int> baseMapDig_;							// 
	std::vector<unsigned int*> mapDig_;								// Dig ... �@��
	std::list<Vector2> digPosList_;									// �@�����ʒu�̋L�����X�g

	// std::vector<std::vector<unsigned int>> mapID_;				// vector����vector�����邱�Ƃɂ���ĂQ�����z�񂪎����\�i���ʂ̂Q�����z��ƈႢvector�P�ЂƂ̂���ėv�f����ς�����j


	// �ϐ�
	//-------
	MapChipGP chipGP_;

	int CrL_;					// �����̐F
	int CrR_;					// �E���̐F
	int goldCnt_;				// MAP���̋���̐�
	int animCnt_;				// �w�i�A�j���[�V�����p�J�E���^

	std::weak_ptr<Camera> camera_;		// Camera��weak���

	// �F�B����
	//-----------
	friend class Camera;				// Camera��MapCtl��private�ɂ�����悤�ɂ���		�u���O�̂��͉̂��̂��́v
	friend class GameScene;
};
