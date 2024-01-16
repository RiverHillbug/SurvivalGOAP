#include "stdafx.h"
#include "GrabMedkitAction.h"
#include "BlackboardTypes.h"

GrabMedkitAction::GrabMedkitAction()
	: GrabItemAction()
{
	AddPrecondition(HAS_TARGET_MEDKIT_PARAM, true);
	AddPrecondition(HAS_MEDKIT_PARAM, false);

	AddEffect(HAS_MEDKIT_PARAM, true);
	AddPlanOnlyEffect(HAS_TARGET_MEDKIT_PARAM, false);
}
