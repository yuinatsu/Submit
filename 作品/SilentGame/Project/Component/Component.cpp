#include "Component.h"

Component::Component(Obj& owner) :
	owner_{owner}
{
}

Component::~Component()
{
}

// �X�V����
void Component::Update(float delta)
{
	// �������Ȃ�
}

void Component::Update(float delta, std::string key)
{
}

