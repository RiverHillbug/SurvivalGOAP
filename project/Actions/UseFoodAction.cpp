#include "stdafx.h"
#include "UseFoodAction.h"
#include "BlackboardTypes.h"

UseFoodAction::UseFoodAction()
	: UseItemAction()
{
	AddPrecondition(HAS_FOOD_PARAM, true);
	AddPrecondition(HAS_HIGH_ENERGY_PARAM, false);

	//AddEffect(STAY_ALIVE_PARAM, true);
	AddEffect(HAS_HIGH_ENERGY_PARAM, false);
}
