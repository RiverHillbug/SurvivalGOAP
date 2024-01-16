#include "stdafx.h"
#include "DestroyGarbageAction.h"
#include "SurvivalAgentPlugin.h"
#include <Exam_HelperStructs.h>
#include <IExamInterface.h>

DestroyGarbageAction::DestroyGarbageAction()
	: GrabItemAction()
{
	AddPrecondition(HAS_TARGET_GARBAGE_PARAM, true);

	AddPlanOnlyEffect(DESTROY_GARBAGE_PARAM, false);

	m_Cost = 1;
}

bool DestroyGarbageAction::GrabItem(SurvivalAgentPlugin* pAgent, const UINT slot, const ItemInfo& item) const
{
	return pAgent->DestroyItem(item);
}
