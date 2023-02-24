#include <type_traits>
#include "Anim_ID.h"


Char_Anim_ID begin(Char_Anim_ID)
{
	return Char_Anim_ID::Idle;
}

Char_Anim_ID end(Char_Anim_ID)
{
	return Char_Anim_ID::MAX;
}

Char_Anim_ID operator*(Char_Anim_ID id)
{
	return id;
}

Char_Anim_ID operator++(Char_Anim_ID& id)
{
	return id = static_cast<Char_Anim_ID>(std::underlying_type<Char_Anim_ID>::type(id) + 1);
}