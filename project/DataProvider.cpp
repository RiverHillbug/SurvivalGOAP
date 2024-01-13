#include "stdafx.h"
#include "DataProvider.h"
#include "SurvivalAgentPlugin.h"
#include "EliteAI\EliteData\EBlackboard.h"
#include "BlackboardTypes.h"
#include "IExamInterface.h"
#include "Helpers.h"
#include <iostream>
#include <unordered_set>

std::unordered_map<std::string, bool> DataProvider::GetWorldState(Elite::Blackboard* pBlackboard)
{
	std::unordered_map<std::string, bool> worldState;

	const SurvivalAgentPlugin* pAgent{ Helpers::GetAgent(pBlackboard) };
	if (pAgent == nullptr)
	{
		std::cout << "Agent not found in blackboard.\n";
		return worldState;
	}

	EnterData(HAS_ENEMY_IN_SIGHT_PARAM, pAgent->GetInterface()->FOV_GetStats().NumEnemies > 0, pBlackboard, worldState);

	CheckHouses(pAgent, pBlackboard, worldState);
	CheckItems(pAgent, pBlackboard, worldState);
	CheckStats(pAgent, pBlackboard, worldState);
	CheckInventory(pAgent, pBlackboard, worldState);

	return worldState;
}

void DataProvider::CheckHouses(const SurvivalAgentPlugin* pAgent, Elite::Blackboard* pBlackboard, OUT WorldState& worldState)
{
	std::vector<HouseInfo> housesInFOV{ pAgent->GetInterface()->GetHousesInFOV() };

	std::unordered_set<HouseInfo> searchedHouses;
	pBlackboard->GetData(SEARCHED_HOUSES_PARAM, searchedHouses);

	Helpers::ExcludeSearchedHouses(housesInFOV, searchedHouses);
	EnterData(HAS_HOUSE_IN_SIGHT_PARAM, !housesInFOV.empty(), pBlackboard, worldState);
}

void DataProvider::CheckItems(const class SurvivalAgentPlugin* pAgent, Elite::Blackboard* pBlackboard, OUT WorldState& worldState)
{
	bool hasWeaponInSight{ false };
	bool hasFoodInSight{ false };
	bool hasMedkitInSight{ false };

	const std::vector<ItemInfo> itemsInFOV{ pAgent->GetInterface()->GetItemsInFOV() };

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
}

void DataProvider::CheckStats(const class SurvivalAgentPlugin* pAgent, Elite::Blackboard* pBlackboard, OUT WorldState& worldState)
{
	const AgentInfo& agentInfo{ pAgent->GetInterface()->Agent_GetInfo() };

	const float highHealthThreshold{ 5.0f };
	EnterData(HAS_HIGH_HEALTH_PARAM, agentInfo.Health > highHealthThreshold, pBlackboard, worldState);

	const float highEnergyThreshold{ 5.0f };
	EnterData(HAS_HIGH_ENERGY_PARAM, agentInfo.Energy > highEnergyThreshold, pBlackboard, worldState);

	const float highStaminaThreshold{ 5.0f };
	EnterData(HAS_HIGH_STAMINA_PARAM, agentInfo.Stamina > highStaminaThreshold, pBlackboard, worldState);

	const float lowStaminaThreshold{ 2.0f };
	EnterData(HAS_LOW_STAMINA_PARAM, agentInfo.Stamina < lowStaminaThreshold, pBlackboard, worldState);
}

void DataProvider::CheckInventory(const class SurvivalAgentPlugin* pAgent, Elite::Blackboard* pBlackboard, OUT WorldState& worldState)
{
	EnterData(HAS_INVENTORY_SPACE_PARAM, pAgent->HasInventorySpace(), pBlackboard, worldState);

	std::vector<UINT> usedWeaponSlots;
	EnterData(HAS_WEAPON_PARAM, pBlackboard->GetData(WEAPON_SLOTS_PARAM, usedWeaponSlots) && !usedWeaponSlots.empty(), pBlackboard, worldState);

	std::vector<UINT> usedFoodSlots;
	EnterData(HAS_FOOD_PARAM, pBlackboard->GetData(FOOD_SLOTS_PARAM, usedFoodSlots) && !usedFoodSlots.empty(), pBlackboard, worldState);

	std::vector<UINT> usedMedkitSlots;
	EnterData(HAS_MEDKIT_PARAM, pBlackboard->GetData(MEDKIT_SLOTS_PARAM, usedMedkitSlots) && !usedMedkitSlots.empty(), pBlackboard, worldState);
}

void DataProvider::EnterData(const std::string& name, const bool value, Elite::Blackboard* pBlackboard, OUT WorldState& worldState)
{
	pBlackboard->SetData(name, value);
	worldState.insert({ name, value });
}
