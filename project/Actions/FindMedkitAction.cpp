#include "stdafx.h"
#include "FindMedkitAction.h"
#include "BlackboardTypes.h"
#include "Exam_HelperStructs.h"

FindMedkitAction::FindMedkitAction()
	: FindItemAction()
{
	AddPrecondition(HAS_MEDKIT_IN_SIGHT_PARAM, true);
	AddPrecondition(HAS_TARGET_MEDKIT_PARAM, false);

	AddPlanOnlyEffect(HAS_TARGET_MEDKIT_PARAM, true);
}

bool FindMedkitAction::IsCorrectItemType(const ItemInfo& item) const
{
	return item.Type == eItemType::MEDKIT;
}
