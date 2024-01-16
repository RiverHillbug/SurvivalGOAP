#include "stdafx.h"
#include "FindGarbageAction.h"
#include "BlackboardTypes.h"
#include "Exam_HelperStructs.h"

FindGarbageAction::FindGarbageAction()
	: FindItemAction()
{
	AddPrecondition(HAS_GARBAGE_IN_MEMORY_PARAM, true);
	AddPrecondition(HAS_TARGET_GARBAGE_PARAM, false);

	AddPlanOnlyEffect(HAS_TARGET_GARBAGE_PARAM, true);
}
