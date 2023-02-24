#include "Component.h"

Component::Component(Obj& owner) :
	owner_{owner}
{
}

Component::~Component()
{
}

// XVˆ—
void Component::Update(float delta)
{
	// ‰½‚à‚µ‚È‚¢
}

void Component::Update(float delta, std::string key)
{
}

