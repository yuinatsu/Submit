#pragma once
#include <map>
#include <list>
#include "../Obj.h"

class Effect :
    public Obj
{
public:
	/// <summary> �R���X�g���N�^ </summary>
	/// <param name="effect"> �G�t�F�N�g�� </param>
	/// <param name="pos"> �G�t�F�N�g�������W </param>
	Effect(EffectName effect, const Math::Vector2& pos);
	Effect(EffectName_x120 effect, const Math::Vector2& pos);
	Effect(EffectNameWhole effect, const Math::Vector2& pos);
	~Effect();
private:
	/// <summary> �X�V </summary>
	/// <param name="delta"> �f���^�^�C�� </param>
	void Update(float delta) override;

	/// <summary> �`�� </summary>
	/// <param name="offset"> �`��I�t�Z�b�g </param>
	/// <param name="drawMng"> �`��}�l�[�W���[ </param>
	void Draw(const Math::Vector2& offset, DrawMng& drawMng) override;

	/// <summary> ID�擾 </summary>
	/// <returns> ObjID::Effect </returns>
	ObjID GetObjID(void) override { return ObjID::Effect; };
};

