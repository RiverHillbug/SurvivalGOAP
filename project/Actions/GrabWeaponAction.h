#pragma once
#include "GrabItemAction.h"
#include "BlackboardTypes.h"

class GrabWeaponAction : public GrabItemAction
{
public:
	GrabWeaponAction();

	const std::string& GetItemTypeSlotParam() const override { return WEAPON_SLOTS_PARAM; }
};
