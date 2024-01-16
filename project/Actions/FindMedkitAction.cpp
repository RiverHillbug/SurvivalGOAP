#include "stdafx.h"
#include "FindMedkitAction.h"
#include "BlackboardTypes.h"
#include "Exam_HelperStructs.h"

FindMedkitAction::FindMedkitAction()
	: FindItemAction()
{
	AddPrecondition(HAS_MEDKIT_IN_MEMORY_PARAM, true);
	AddPrecondition(HAS_TARGET_MEDKIT_PARAM, false);

	AddPlanOnlyEffect(HAS_TARGET_MEDKIT_PARAM, true);
}
