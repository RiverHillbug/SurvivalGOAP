#include "stdafx.h"
#include "UseItemAction.h"
#include "BlackboardTypes.h"
#include "SurvivalAgentPlugin.h"
#include "IExamInterface.h"
#include "EliteAI\EliteData\EBlackboard.h"
#include "Helpers.h"
#include <vector>

UseItemAction::UseItemAction()
	: GOAPAction()
{
	m_Cost = 2;
}

bool UseItemAction::Perform(Elite::Blackboard* pBlackboard) const
{
	SurvivalAgentPlugin* pAgent{ Helpers::GetAgent(pBlackboard) };
	if (pAgent == nullptr)
		return false;

	const std::string& itemTypeSlotParam{ GetItemTypeSlotParam() };

	std::vector<UINT> usedSlots{};
	if (!pBlackboard->GetData(itemTypeSlotParam, usedSlots) || usedSlots.empty())
	{
		std::cout << "No valid inventory slot given to use item!\n";
		return false;
	}

	if (m_NeedsToFaceTarget)
	{
		FaceTarget(pAgent, pBlackboard);
	}

	const UINT itemSlot{ usedSlots.back() };

	if (!pAgent->GetInterface()->Inventory_UseItem(itemSlot))
		return false;

	ItemInfo itemInfo;
	if (!pAgent->GetInterface()->Inventory_GetItem(itemSlot, itemInfo))
		return false;

	if (itemInfo.Value == 0)
	{
		pAgent->GetInterface()->Inventory_RemoveItem(itemSlot);
		usedSlots.pop_back();

		pBlackboard->SetData(itemTypeSlotParam, usedSlots);
	}

	return true;
}

bool UseItemAction::IsDone(const Elite::Blackboard* pBlackboard) const
{
	// We are done after one update regardless
	return true;
}
