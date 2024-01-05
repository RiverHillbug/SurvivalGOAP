#include "stdafx.h"
#include "UseItemAction.h"
#include "BlackboardTypes.h"
#include "SurvivalAgentPlugin.h"
#include "IExamInterface.h"
#include "EliteAI\EliteData\EBlackboard.h"

UseItemAction::UseItemAction()
	: GOAPAction()
{
	m_Cost = 1;
}

bool UseItemAction::Perform(Elite::Blackboard* pBlackboard)
{
	SurvivalAgentPlugin* pAgent{ nullptr };
	if (pBlackboard->GetData(AGENT_PARAM, pAgent))
	{
		ItemInfo itemToUse;
		pBlackboard->GetData(TARGET_ITEM_PARAM, itemToUse);
		UINT slot{ INVALID_INVENTORY_SLOT };

		switch (itemToUse.Type)
		{
		case eItemType::PISTOL:
		case eItemType::SHOTGUN:
			pBlackboard->GetData(WEAPON_SLOT_PARAM, slot);
			break;

		case eItemType::MEDKIT:
			pBlackboard->GetData(MEDKIT_SLOT_PARAM, slot);
			break;

		case eItemType::FOOD:
			pBlackboard->GetData(FOOD_SLOT_PARAM, slot);
		}

		if (slot != INVALID_INVENTORY_SLOT)
			pAgent->GetInterface()->Inventory_UseItem(slot);
	
	}

	return false;
}
