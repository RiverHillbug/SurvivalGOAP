#include "stdafx.h"
#include "UseMedkitAction.h"
#include "BlackboardTypes.h"

UseMedkitAction::UseMedkitAction()
	: UseItemAction()
{
	AddPrecondition(HAS_MEDKIT_PARAM, true);
	AddPrecondition(HAS_HIGH_HEALTH_PARAM, false);

	AddPlanOnlyEffect(HAS_HIGH_HEALTH_PARAM, true);
}
