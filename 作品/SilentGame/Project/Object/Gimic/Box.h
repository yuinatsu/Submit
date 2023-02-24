#pragma once
#include <list>
#include "../Obj.h"
#include "../../common/Math.h"


class ObjManager;
class UiBase;
class TmxObj;

enum class ItemName;

class Box :
	public Obj
{
public:
	Box(std::shared_ptr<TmxObj>& tmxObj,UiBase& itemUi,const Math::Vector2& pos, ObjManager& objMng, ItemName itemName);
	~Box();
	void Init(void);
	//void Update(float delta) override;
	void Draw(const Math::Vector2& drawoffset, DrawMng& drawMng) override;
	bool GetFlag(void);
	ItemName GetItemName(void);
private:

	ObjID GetObjID(void) override { return ObjID::Box; }

	void Hit(Collider& other);

	// �󂢂Ă��邩�t���O
	bool flag_;
	// �����Ă���A�C�e���̎��
	ItemName itemName_;

	// �I�u�W�F�N�g�}�l�[�W���[
	ObjManager& objMng_;

	UiBase& itemUi_;

	std::string imgKey_;

	std::shared_ptr<TmxObj> tmxObj_;
};

