#include "stdafx.h"
#include "DataProvider.h"
#include "SurvivalAgentPlugin.h"
#include "EliteAI\EliteData\EBlackboard.h"
#include "BlackboardTypes.h"
#include "IExamInterface.h"

std::unordered_map<std::string, bool> DataProvider::GetWorldState(Elite::Blackboard* pBlackboard)
{
	std::unordered_map<std::string, bool> worldState;

	SurvivalAgentPlugin* pAgent;
	if (!pBlackboard->GetData(AGENT_PARAM, pAgent))
	{
		// ERROR
		return worldState;
	}

	IExamInterface* pInterface{ pAgent->GetInterface() };

	if (pAgent->SelectFirstAvailableInventorySlot() != INVALID_INVENTORY_SLOT)
	{
		pBlackboard->SetData(HAS_INVENTORY_SPACE_PARAM, true);
		worldState.insert({ HAS_INVENTORY_SPACE_PARAM, true });
	}

	const bool hasEnemiesInSight{ true/*pInterface->FOV_GetStats().NumEnemies > 0*/ };
	pBlackboard->SetData(HAS_ENEMY_IN_SIGHT_PARAM, hasEnemiesInSight);
	worldState.insert({ HAS_ENEMY_IN_SIGHT_PARAM, hasEnemiesInSight });

	const bool hasHouseInSight{ pInterface->FOV_GetStats().NumHouses > 0 };
	pBlackboard->SetData(HAS_HOUSE_IN_SIGHT_PARAM, hasHouseInSight);
	worldState.insert({ HAS_HOUSE_IN_SIGHT_PARAM, hasHouseInSight });

	pBlackboard->SetData(HAS_WEAPON_IN_SIGHT_PARAM, true);
	pBlackboard->SetData(HAS_MEDKIT_IN_SIGHT_PARAM, true);
	pBlackboard->SetData(HAS_FOOD_IN_SIGHT_PARAM, true);

	const std::vector<ItemInfo> itemsInFOV{ pInterface->GetItemsInFOV() };
	for (const auto& item : itemsInFOV)
	{
		switch (item.Type)
		{
		case eItemType::SHOTGUN:
		case eItemType::PISTOL:
			pBlackboard->SetData(HAS_WEAPON_IN_SIGHT_PARAM, true);
			break;

		case eItemType::MEDKIT:
			pBlackboard->SetData(HAS_MEDKIT_IN_SIGHT_PARAM, true);
			break;

		case eItemType::FOOD:
			pBlackboard->SetData(HAS_FOOD_IN_SIGHT_PARAM, true);
			break;
		}
	}

	worldState.insert({ HAS_WEAPON_IN_SIGHT_PARAM, pBlackboard->GetBoolData(HAS_WEAPON_IN_SIGHT_PARAM) });
	worldState.insert({ HAS_MEDKIT_IN_SIGHT_PARAM, pBlackboard->GetBoolData(HAS_MEDKIT_IN_SIGHT_PARAM) });
	worldState.insert({ HAS_FOOD_IN_SIGHT_PARAM, pBlackboard->GetBoolData(HAS_FOOD_IN_SIGHT_PARAM) });

	return worldState;
}
