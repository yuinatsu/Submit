#pragma once
#include <memory>
#include <list>
#include <vector>
#include <fstream>
//#include "../Object/Obj.h"
#include <map>
#include "../common/Math.h"
#include "Effect/EffectID.h"
#include "Item/ItemName.h"
#include "../common/Random.h"
#include "../common/Structure.h"

 	class Obj;
	using BoxItem = std::map<ItemName, bool>;
class TmxObj;
class GmkEvent;
class UiBase;
class BehaviorTreeLoader;
class DrawMng;
class ResultData;
class Footprints;
enum class ObjID;

class ObjManager
{
public:
	ObjManager(
		const std::string& objDataName,
		std::shared_ptr< ResultData>& resultData, 
		UiBase& itemUi,
		std::shared_ptr<GmkEvent>& gmkEvent,
		std::shared_ptr<TmxObj>& tmxObj,
		const Math::Vector2& scrSize
	);

	~ObjManager();

	bool Update(float delta);
	void UpdateEffect(float delta);
	// �`���[�g���A���p�̃A�b�v�f�[�g
	bool UpdateTutorial1(float delta, bool moveFlag, bool hitFlag);
	bool UpdateTutorial2(float delta, bool moveFlag, bool hitFlag, bool enemyDrawFlag);
	bool UpdateTutorial3(float delta, bool moveFlag, bool hitFlag,bool gmcflag);
	// �Q�[���N���Aor�Q�[���I�[�o�[
	bool UpdateGameEnd(float delta);
	void Draw(const Math::Vector2& offset, DrawMng& drawMng);
	void EffectDraw(const Math::Vector2& offset, DrawMng& drawMng);		// Effect�̂ݕ`�悵�������Ɏg�p
	void EffectDraw(DrawMng& drawMng);									// Effect�̂ݕ`��(�I�t�Z�b�g����)
	// �`���[�g���A�����ƂɊ֐�����邯�ǂ܂Ƃ߂邱�Ƃ��ł���̂Ȃ�܂Ƃ߂�
	void DrawTutorial1(const Math::Vector2& offset, DrawMng& drawMng);
	void DrawTutorial2(const Math::Vector2& offset, DrawMng& drawMng, bool enemyFlag);
	void DrawTutorial3(const Math::Vector2& offset, DrawMng& drawMng);
	void DrawGameEnd(const Math::Vector2& offset, DrawMng& drawMng);

	/// <summary> �J�n���o���̍X�V���� </summary>
	/// <param name="delta"> �f���^�^�C�� </param>
	void UpdateStart(float delta);

	/// <summary> ���W�Ɉ�ԋ߂��G��T�� </summary>
	/// <param name="pos"> ���W </param>
	/// <returns> ��ԋ߂��G </returns>
	std::weak_ptr<Obj> FindEnemy(const Math::Vector2& pos);

	void SortEnemyList(const Math::Vector2& pos);

	/// <summary> �w�肳�ꂽ���W�ɋ߂����Ƀ\�[�g���ꂽ�G�̃��X�g��Ԃ� </summary>
	/// <param name="pos"> �\�[�g�����̍��W </param>
	/// <returns> �G�̃��X�g </returns>
	std::list<std::shared_ptr<Obj>>& GetSortedEnemyList(const Math::Vector2& pos);

	/// <summary> �v���C���[���擾 </summary>
	/// <param name=""></param>
	/// <returns> �v���C���[ </returns>
	std::weak_ptr<Obj> GetPlayer(void);


	/// <summary> �^�[�Q�b�g���擾 </summary>
	/// <param name=""></param>
	/// <returns> �^�[�Q�b�g </returns>
	std::weak_ptr<Obj> GetTarget(void);

	std::weak_ptr<GmkEvent> GetGmkEvent(void);
	
	// �ǂ̃A�C�e���������Ă��锠���J������
	BoxItem& GetBoxItem(void);

	// �g�p���ꂽ�A�C�e���̃Z�b�g
	void SetUseItem(ItemName item) { useItem_ = item; };

	// �A�C�e���g�p��
	void UseItem(ItemName item);

	// ����̃A�C�e�����������Ă��邩
	bool HaveSpecItem(ItemName item);

	/// <summary> �G�t�F�N�g��ǉ� </summary>
	/// <param name="effectName"> �G�t�F�N�g�̖��O </param>
	/// <param name="pos"> �������W </param>
	void AddEffect(EffectName effectName, const Math::Vector2& pos);

	/// <summary> �G�t�F�N�g��ǉ� </summary>
	/// <param name="effectName"></param>
	/// <param name="pos"></param>
	void AddEffect(EffectName_x120 effectName, const Math::Vector2& pos);

	void AddEffect(EffectNameWhole effectName, const Math::Vector2& pos);

	bool GetPlayerEntrance(void) { return pEnt_; }

	// �A���[�g���N�����Ă��邩�ǂ���
	bool GetAlertFlag(void);

	bool IsClear(void) { return isClear_; }

	void SetHaveItem(std::vector<std::pair<int, bool>> item) { item_ = item; }

	const std::list<std::unique_ptr<Obj>>& GetItemList(void) const { return itemList_; }
private:

	/// <summary> �I�u�W�F�N�g���m�̓����蔻������ </summary>
	/// <param name=""></param>
	void CheckObjHit(void);
	void CheckObjHitTutorial1(bool hitflag);
	void CheckObjHitTutorial2(bool hitflag);
	void CheckObjHitTutorial3(bool hitflag, bool gmcFlag);
	bool Load(const std::string& xmlName, UiBase& itemUi, std::shared_ptr<GmkEvent>& gmkEvent, std::shared_ptr<TmxObj>& tmxObj);


	// �w�b�_�[
	struct Header
	{
		char s[4];				// �V�O�l�`��
		unsigned int ver;		// �o�[�W����
		unsigned int sum;		// �`�F�b�NSam
	};

	// �f�[�^���p�̃w�b�_�[
	struct DateHeader
	{
		ObjID id;		// �I�u�W�F�N�g��ID
		int num;		// ��
	};

	// �I�u�W�F�N�g�̊�{�f�[�^
	struct ObjData
	{
		Math::Vector2 pos_;		// ���W
		Math::Vector2 vec_;		// ����
		std::string imgName_;	// �摜�̖��O
	};

	/// <summary> �v���C���[�������� </summary>
	/// <param name="tmxObj"> tmxObj </param>
	/// <param name="obj"> �I�u�W�F�N�g�̊�{�f�[�^ </param>
	/// <param name="f"> �t�@�C���X�g���[�� </param>
	void CreatePlayer(std::shared_ptr<TmxObj>& tmxObj, ObjData& obj, std::ifstream& f);

	/// <summary> �G�̐������� </summary>
	/// <param name="bloder"> �r�w�C�r�A�c���[�̃��[�h�p�N���X </param>
	/// <param name="tmxObj"> tmxObj </param>
	/// <param name="obj"> �I�u�W�F�N�g�̊�{�f�[�^ </param>
	/// <param name="f"> �t�@�C���X�g���[�� </param>
	void CreateEnemy(BehaviorTreeLoader& bloder, std::shared_ptr<TmxObj>& tmxObj, ObjData& obj, std::ifstream& f);

	/// <summary> �N���A�ڕW�̐������� </summary>
	/// <param name="bloder"> �r�w�C�r�A�c���[�̃��[�h�p�N���X </param>
	/// <param name="tmxObj"> tmxObj </param>
	/// <param name="obj"> �I�u�W�F�N�g�̊�{�f�[�^ </param>
	/// <param name="f"> �t�@�C���X�g���[�� </param>
	void CreateTarget(BehaviorTreeLoader& bloder, std::shared_ptr<TmxObj>& tmxObj, ObjData& obj, std::ifstream& f);


	void CreateDecoration(std::shared_ptr<TmxObj>& tmxObj,ObjData& obj, std::ifstream& f);

	void CreateDoor(std::shared_ptr<TmxObj>& tmx);

	std::shared_ptr< ResultData> resultData_;

	// �v���C���[
	std::shared_ptr<Obj> player_;

	// �A�C�e��
	std::vector<std::pair<int, bool>> item_;

	// �G���X�g
	std::list<std::shared_ptr<Obj>> enemyList_;

	std::shared_ptr<Obj> target_;

	// �M�~�b�N���X�g
	std::list<std::unique_ptr<Obj>> gimicList_;

	// �A�C�e�����X�g
	std::list<std::unique_ptr<Obj>> itemList_;

	// �G�t�F�N�g���X�g
	std::list<std::unique_ptr<Obj>> effectList_;

	// ���ݎg�p���ꂽ�A�C�e��
	ItemName useItem_;

	// �M�~�b�N�C�x���g�N���X
	std::weak_ptr<GmkEvent> gmkEvent_;

	// �Đ��҂��̃G�t�F�N�g
	EffectName effect_;

	Random random_;

	// �`���[�g���A���ŕ\�����邩�ǂ���
	bool pflag_;
	bool eflag_;
	BoxItem boxItem_;
	bool alertFlag_;

	// �v���C���[����ʂɕ\��Ă��邩�ǂ���
	bool pEnt_;

	// �N���A����
	bool isClear_;

	std::list<std::unique_ptr<Footprints>> footprintsList_;

	std::list<std::unique_ptr<Obj>> doorList_;
};