#include "stdafx.h"
#include "ExploreAction.h"
#include "EliteAI/EliteData/EBlackboard.h"

ExploreAction::ExploreAction()
{

}

bool ExploreAction::Perform(Elite::Blackboard* pBlackboard) const
{
	return false;
}

bool ExploreAction::IsDone(const Elite::Blackboard* pBlackboard) const
{
	return false;
}
