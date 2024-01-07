#include "stdafx.h"
#include "GrabFoodAction.h"
#include "BlackboardTypes.h"

GrabFoodAction::GrabFoodAction()
	: GrabItemAction()
{
	AddPrecondition(HAS_TARGET_FOOD_PARAM, true);

	AddEffect(HAS_FOOD_PARAM, true);
	AddPlanOnlyEffect(HAS_TARGET_FOOD_PARAM, false);
}
