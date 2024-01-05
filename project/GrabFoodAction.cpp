#include "stdafx.h"
#include "GrabFoodAction.h"
#include "BlackboardTypes.h"

GrabFoodAction::GrabFoodAction()
	: GrabItemAction()
{
	AddPrecondition(HAS_FOOD_PARAM, false);
	AddEffect(HAS_FOOD_PARAM, true);
}
