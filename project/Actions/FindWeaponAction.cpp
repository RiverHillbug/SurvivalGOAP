#include "stdafx.h"
#include "FindWeaponAction.h"
#include "BlackboardTypes.h"
#include "Exam_HelperStructs.h"

FindWeaponAction::FindWeaponAction()
	: FindItemAction()
{
	AddPrecondition(HAS_WEAPON_IN_MEMORY_PARAM, true);
	AddPrecondition(HAS_TARGET_WEAPON_PARAM, false);

	AddPlanOnlyEffect(HAS_TARGET_WEAPON_PARAM, true);
}
