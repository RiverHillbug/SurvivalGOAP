#include "stdafx.h"
#include "GrabItemAction.h"
#include "BlackboardTypes.h"
#include "SurvivalAgentPlugin.h"
#include "IExamInterface.h"
#include "EliteAI\EliteData\EBlackboard.h"

GrabItemAction::GrabItemAction()
	: GOAPAction()
{
	AddPrecondition(HAS_INVENTORY_SPACE_PARAM, true);

	m_Cost = 3;
	m_RequiresInRange = true;
}

void GrabItemAction::Reset()
{
	m_IsDone = false;
}

bool GrabItemAction::Perform(Elite::Blackboard* pBlackboard) const
{
	SurvivalAgentPlugin* pAgent{ nullptr };
	if (pBlackboard->GetData(AGENT_PARAM, pAgent))
	{

		ItemInfo item;
		pBlackboard->GetData(TARGET_ITEM_PARAM, item);

		pAgent->GetInterface()->GrabItem(item);
		const UINT slot{ pAgent->SelectFirstAvailableInventorySlot() };

		if (slot != INVALID_INVENTORY_SLOT)
		{
			pAgent->Inventory_AddItem(slot, item);
			
			switch (item.Type)
			{
			case eItemType::PISTOL:
			case eItemType::SHOTGUN:
				pBlackboard->SetData(WEAPON_SLOT_PARAM, slot);
				break;

			case eItemType::MEDKIT:
				pBlackboard->SetData(MEDKIT_SLOT_PARAM, slot);
				break;

			case eItemType::FOOD:
				pBlackboard->SetData(FOOD_SLOT_PARAM, slot);
			}

			//m_IsDone = true;
			return true;
		}
	}

	return false;
}
