#include "stdafx.h"
#include "GrabWeaponAction.h"
#include "BlackboardTypes.h"

GrabWeaponAction::GrabWeaponAction()
	: GrabItemAction()
{
	//AddPrecondition(HAS_WEAPON_PARAM, false);
	AddEffect(HAS_WEAPON_PARAM, true);
	AddEffect(HAS_TARGET_ITEM_PARAM, false);
}
