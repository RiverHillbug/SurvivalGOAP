#include "stdafx.h"
#include "GrabItemAction.h"
#include "BlackboardTypes.h"
#include "SurvivalAgentPlugin.h"
#include "IExamInterface.h"
#include "EliteAI\EliteData\EBlackboard.h"
#include "Helpers.h"
#include <iostream>
#include <vector>
#include <xutility>

GrabItemAction::GrabItemAction()
	: GOAPAction()
{
	AddPrecondition(HAS_INVENTORY_SPACE_PARAM, true);
	AddPlanOnlyEffect(FILL_INVENTORY_SPACE_PARAM, true);

	m_Cost = 3;
	m_RequiresInRange = true;
}

bool GrabItemAction::TryGetDestination(const Elite::Blackboard* pBlackboard, OUT Elite::Vector2& destination) const
{
	ItemInfo item;
	if (pBlackboard->GetData(TARGET_ITEM_PARAM, item))
	{
		destination = item.Location;
		return true;
	}

	return false;
}

bool GrabItemAction::Perform(Elite::Blackboard* pBlackboard) const
{
	SurvivalAgentPlugin* pAgent{ Helpers::GetAgent(pBlackboard) };
	if (pAgent == nullptr)
		return false;

	const UINT slot{ pAgent->SelectFirstAvailableInventorySlot() };
	if (slot == INVALID_INVENTORY_SLOT)
	{
		std::cout << "No valid inventory slot to store item!\n";
		return false;
	}

	ItemInfo targetItem;
	if (!pBlackboard->GetData(TARGET_ITEM_PARAM, targetItem))
	{
		std::cout << "No valid target item in blackboard!\n";
		return false;
	}

	if (!pAgent->GetInterface()->GrabItem(targetItem))
		return false;

	if (!pAgent->Inventory_AddItem(slot, targetItem))
		return false;

	const std::string& itemTypeSlotParam{ GetItemTypeSlotParam() };

	std::vector<UINT> usedSlots;
	pBlackboard->GetData(itemTypeSlotParam, usedSlots);
	usedSlots.push_back(slot);

	pBlackboard->SetData(itemTypeSlotParam, usedSlots);

	return true;
}

bool GrabItemAction::IsDone(const Elite::Blackboard* pBlackboard) const
{
	const SurvivalAgentPlugin* pAgent{ Helpers::GetAgent(pBlackboard) };
	if (pAgent == nullptr)
		return true;

	std::vector<UINT> usedSlots;
	return pBlackboard->GetData(GetItemTypeSlotParam(), usedSlots) &&
		std::ranges::find(usedSlots, pAgent->GetSelectedInventorySlot()) != usedSlots.end();
}
