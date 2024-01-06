#include "stdafx.h"
#include "UseMedkitAction.h"
#include "BlackboardTypes.h"

UseMedkitAction::UseMedkitAction()
	: UseItemAction()
{
	AddPrecondition(HAS_LOW_HEALTH_PARAM, true);
	AddPrecondition(HAS_MEDKIT_PARAM, true);
	AddEffect(HAS_LOW_HEALTH_PARAM, false);
	AddEffect(STAY_ALIVE_PARAM, true);
}
