#include "stdafx.h"
#include "FindItemAction.h"
#include "BlackboardTypes.h"
#include "IExamInterface.h"
#include "SurvivalAgentPlugin.h"
#include "Exam_HelperStructs.h"
#include "EliteAI\EliteData\EBlackboard.h"
#include "EliteMath\EVector2.h"
#include <algorithm>

FindItemAction::FindItemAction()
	: GOAPAction()
{
	AddPrecondition(HAS_INVENTORY_SPACE_PARAM, true);

	m_Cost = 1;
}

bool FindItemAction::Perform(Elite::Blackboard* pBlackboard) const
{
	SurvivalAgentPlugin* pAgent{ nullptr };
	if (pBlackboard->GetData(AGENT_PARAM, pAgent))
	{
		const std::vector<ItemInfo> allItemsInFOV{ pAgent->GetInterface()->GetItemsInFOV() };
		std::vector<ItemInfo> correctItemsInFOV{ allItemsInFOV.size() };

		// Copy all items that are of the correct type from allItemsInFOV into correctItemsInFOV
		std::ranges::copy_if(allItemsInFOV, std::back_inserter(correctItemsInFOV),
			[this](const ItemInfo& item)
			{
				return IsCorrectItemType(item);
			});

		const Elite::Vector2 agentLocation{ pAgent->GetInterface()->Agent_GetInfo().Position };

		const ItemInfo closestWeapon{ *std::ranges::min_element(correctItemsInFOV,
			[agentLocation](const ItemInfo& left, const ItemInfo& right)
			{
				const float distanceLeft{ (left.Location - agentLocation).MagnitudeSquared() };
				const float distanceRight{ (right.Location - agentLocation).MagnitudeSquared() };

				return distanceLeft < distanceRight;
			}) };

		pBlackboard->SetData(TARGET_ITEM_PARAM, closestWeapon);

		//m_IsDone = true;
		return true;
	}

	//m_IsDone = true;
	return false;
}
