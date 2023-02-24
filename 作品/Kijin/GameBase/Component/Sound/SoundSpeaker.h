#pragma once
#include "../ComponentBase.h"
#include "../../Common/SharedHandle.h"


class SoundSpeaker :
    public ComponentBase
{
public:

	void Play(void);

	ComponentID GetID(void) const override
	{
		return id_;
	}
	SetID(ComponentID::Base, ComponentID::SoundSpeaker)
private:

};

