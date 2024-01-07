#include "stdafx.h"
#include "GrabWeaponAction.h"
#include "BlackboardTypes.h"

GrabWeaponAction::GrabWeaponAction()
	: GrabItemAction()
{
	AddPrecondition(HAS_TARGET_WEAPON_PARAM, true);

	AddEffect(HAS_WEAPON_PARAM, true);
	AddEffect(HAS_TARGET_WEAPON_PARAM, false);
}
