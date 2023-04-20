#pragma once
#include <map>
#include <list>
#include "../Obj.h"

class Effect :
    public Obj
{
public:
	// �R���X�g���N�^ 
	Effect(EffectName effect, const Math::Vector2& pos);
	Effect(EffectName_x120 effect, const Math::Vector2& pos);
	Effect(EffectNameWhole effect, const Math::Vector2& pos);
	~Effect();
private:
	// �X�V 
	void Update(float delta) override;

	// �`�� 
	void Draw(const Math::Vector2& offset, DrawMng& drawMng) override;

	// ID�擾 
	ObjID GetObjID(void) override { return ObjID::Effect; };
};

