#include "stdafx.h"
#include "FindFoodAction.h"
#include "BlackboardTypes.h"
#include "Exam_HelperStructs.h"

FindFoodAction::FindFoodAction()
	: FindItemAction()
{
	AddPrecondition(HAS_FOOD_IN_SIGHT_PARAM, true);
	AddPrecondition(HAS_TARGET_FOOD_PARAM, false);

	AddPlanOnlyEffect(HAS_TARGET_FOOD_PARAM, true);
}

bool FindFoodAction::IsCorrectItemType(const ItemInfo& item) const
{
	return item.Type == eItemType::FOOD;
}
