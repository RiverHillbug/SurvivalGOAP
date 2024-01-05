#include "stdafx.h"
#include "UseWeaponAction.h"
#include "BlackboardTypes.h"

UseWeaponAction::UseWeaponAction()
	: UseItemAction()
{
	AddPrecondition(HAS_WEAPON_PARAM, true);
	AddPrecondition(HAS_ENEMY_IN_SIGHT_PARAM, true);
	AddEffect(KILL_ENEMY_PARAM, true);
	AddEffect(STAY_ALIVE_PARAM, true);
}
