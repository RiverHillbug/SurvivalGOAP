#include "stdafx.h"
#include "FindItemAction.h"
#include "BlackboardTypes.h"
#include "IExamInterface.h"
#include "SurvivalAgentPlugin.h"
#include "Exam_HelperStructs.h"
#include "EliteAI\EliteData\EBlackboard.h"
#include "EliteMath\EVector2.h"
#include "Helpers.h"
#include <algorithm>

FindItemAction::FindItemAction()
	: GOAPAction()
{
	AddPrecondition(HAS_INVENTORY_SPACE_PARAM, true);

	m_Cost = 1;
}

bool FindItemAction::Perform(Elite::Blackboard* pBlackboard) const
{
	const SurvivalAgentPlugin* pAgent{ Helpers::GetAgent(pBlackboard) };
	if (pAgent == nullptr)
		return false;

	const std::vector<ItemInfo> allItemsInFOV{ pAgent->GetInterface()->GetItemsInFOV() };
	std::vector<ItemInfo> correctItemsInFOV{};

	// Copy all items that are of the correct type from allItemsInFOV into correctItemsInFOV
	std::ranges::copy_if(allItemsInFOV, std::back_inserter(correctItemsInFOV),
		[this](const ItemInfo& item)
		{
			return IsCorrectItemType(item);
		});

	if (correctItemsInFOV.empty())
	{
		std::cout << "No correct item in sight!\n";
		return false;
	}

	const Elite::Vector2 agentLocation{ pAgent->GetInterface()->Agent_GetInfo().Position };
	const ItemInfo closestItem{ Helpers::GetClosestFromPosition<ItemInfo>(correctItemsInFOV, agentLocation, [](const ItemInfo& item) { return item.Location; }) };

	pBlackboard->SetData(TARGET_ITEM_PARAM, closestItem);
	return true;
}

bool FindItemAction::IsDone(const Elite::Blackboard* pBlackboard) const
{
	const SurvivalAgentPlugin* pAgent{ Helpers::GetAgent(pBlackboard) };
	if (pAgent == nullptr)
		return true;

	ItemInfo targetWeapon;
	return pBlackboard->GetData(TARGET_ITEM_PARAM, targetWeapon);
}
