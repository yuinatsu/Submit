#pragma once
#include "../Obj.h"

class Decoy :
	public Obj
{
public:
	Decoy(const Math::Vector2& pos);
	~Decoy();
private:
	bool Init(void);
	void Update(float delta) override;
	void Draw(const Math::Vector2& offset, DrawMng& drawMng)override;
	ObjID GetObjID(void) override { return ObjID::Decoy; };
};

