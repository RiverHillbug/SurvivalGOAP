#include "stdafx.h"
#include "DataProvider.h"
#include "SurvivalAgentPlugin.h"
#include "EliteAI\EliteData\EBlackboard.h"
#include "BlackboardTypes.h"
#include "IExamInterface.h"
#include "Helpers.h"
#include "Memory.h"
#include <iostream>
#include <unordered_set>

std::unordered_map<std::string, bool> DataProvider::GetWorldState(Elite::Blackboard* pBlackboard, const Memory& memory)
{
	std::unordered_map<std::string, bool> worldState;

	const SurvivalAgentPlugin* pAgent{ Helpers::GetAgent(pBlackboard) };
	if (pAgent == nullptr)
		return worldState;

	EnterData(HAS_ENEMY_IN_MEMORY_PARAM, !memory.GetEnemiesInMemory().empty(), pBlackboard, worldState);
	EnterData(HAS_WEAPON_IN_MEMORY_PARAM, !memory.GetWeaponsInMemory().empty(), pBlackboard, worldState);
	EnterData(HAS_FOOD_IN_MEMORY_PARAM, !memory.GetFoodsInMemory().empty(), pBlackboard, worldState);
	EnterData(HAS_MEDKIT_IN_MEMORY_PARAM, !memory.GetMedkitsInMemory().empty(), pBlackboard, worldState);
	EnterData(HAS_GARBAGE_IN_MEMORY_PARAM, !memory.GetGarbageInMemory().empty(), pBlackboard, worldState);
	EnterData(HAS_HOUSE_IN_MEMORY_PARAM, !memory.GetUnsearchedHouses().empty(), pBlackboard, worldState);
	EnterData(HAS_PURGE_ZONE_IN_MEMORY_PARAM, !memory.GetPurgeZonesInMemory().empty(), pBlackboard, worldState);

	CheckStats(pAgent, pBlackboard, worldState);
	CheckInventory(pAgent, pBlackboard, worldState);

	return worldState;
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

	EnterData(IS_NOT_IN_PURGE_ZONE_PARAM, !Helpers::IsAgentInPurgeZone(pBlackboard), pBlackboard, worldState);
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
