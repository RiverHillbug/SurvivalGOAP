#include "stdafx.h"
#include "DataProvider.h"
#include "SurvivalAgentPlugin.h"
#include "EliteAI\EliteData\EBlackboard.h"
#include "BlackboardTypes.h"
#include "IExamInterface.h"
#include <iostream>

std::unordered_map<std::string, bool> DataProvider::GetWorldState(Elite::Blackboard* pBlackboard)
{
	std::unordered_map<std::string, bool> worldState;

	SurvivalAgentPlugin* pAgent;
	if (!pBlackboard->GetData(AGENT_PARAM, pAgent))
	{
		std::cout << "Agent not found in blackboard.\n";
		return worldState;
	}

	const IExamInterface* pInterface{ pAgent->GetInterface() };

	EnterData(HAS_INVENTORY_SPACE_PARAM, pAgent->HasInventorySpace(), pBlackboard, worldState);

	EnterData(HAS_ENEMY_IN_SIGHT_PARAM, false /*pInterface->FOV_GetStats().NumEnemies > 0*/, pBlackboard, worldState);
	EnterData(HAS_HOUSE_IN_SIGHT_PARAM, true /*pInterface->FOV_GetStats().NumHouses > 0*/, pBlackboard, worldState);

	bool hasWeaponInSight{ true /*false*/ };
	bool hasFoodInSight{ true /*false*/ };
	bool hasMedkitInSight{ true /*false*/ };

	const std::vector<ItemInfo> itemsInFOV{ pInterface->GetItemsInFOV() };

	for (const auto& item : itemsInFOV)
	{
		switch (item.Type)
		{
		case eItemType::SHOTGUN:
		case eItemType::PISTOL:
			hasWeaponInSight = true;
			break;

		case eItemType::FOOD:
			hasFoodInSight = true;
			break;

		case eItemType::MEDKIT:
			hasMedkitInSight = true;
			break;
		}
	}

	EnterData(HAS_WEAPON_IN_SIGHT_PARAM, hasWeaponInSight, pBlackboard, worldState);
	EnterData(HAS_FOOD_IN_SIGHT_PARAM, hasFoodInSight, pBlackboard, worldState);
	EnterData(HAS_MEDKIT_IN_SIGHT_PARAM, hasMedkitInSight, pBlackboard, worldState);

	EnterData(HAS_HIGH_ENERGY_PARAM, true, pBlackboard, worldState);
	EnterData(HAS_HIGH_HEALTH_PARAM, true, pBlackboard, worldState);

	return worldState;
}

void DataProvider::EnterData(const std::string& name, const bool value, Elite::Blackboard* pBlackboard, OUT WorldState& worldState)
{
	pBlackboard->SetData(name, value);
	worldState.insert({ name, value });
}
