#include "stdafx.h"
#include "SearchHouseAction.h"
#include "SurvivalAgentPlugin.h"
#include "BlackboardTypes.h"
#include "Helpers.h"
#include "Memory.h"
#include <IExamInterface.h>
#include <iostream>
#include <unordered_set>
#include <numeric>

SearchHouseAction::SearchHouseAction(const WorldInfo& worldInfo)
	: ExploreAction(worldInfo)
{
	AddPrecondition(HAS_HOUSE_IN_MEMORY_PARAM, true);
	AddPrecondition(HAS_INVENTORY_SPACE_PARAM, true);
	AddPlanOnlyEffect(FILL_INVENTORY_SPACE_PARAM, true);

	// Less cost than Explore, to ensure we prioritize this over it, but still expensive to avoid doing it over a non-exploring action
	m_Cost = 8;
}

void SearchHouseAction::OnStart(Elite::Blackboard* pBlackboard) const
{
	SelectTargetHouse(pBlackboard);

	ExploreAction::OnStart(pBlackboard);
}

bool SearchHouseAction::Perform(Elite::Blackboard* pBlackboard) const
{
	// We have set the destination in OnEnter, we won't set any new destination for this action so do nothing
	return true;
}

void SearchHouseAction::OnExit(Elite::Blackboard* pBlackboard) const
{
	ExploreAction::OnExit(pBlackboard);

	HouseInfo targetHouse;
	if (!pBlackboard->GetData(TARGET_HOUSE_PARAM, targetHouse))
		return;

	pBlackboard->RemoveData(TARGET_HOUSE_PARAM);

	SurvivalAgentPlugin* pAgent{ Helpers::GetAgent(pBlackboard) };
	if (pAgent == nullptr)
		return;

	const Memory& memory{ pAgent->GetMemory() };

	// Mark the house as searched only if we stop because we saw an item in it or reached its center
	if (memory.HasSeenAnyNew(eEntityType::ITEM) || pAgent->IsApproximatelyAt(pAgent->GetDestination()))
	{
		pAgent->GetMemory().MarkHouseAsSearched(targetHouse);
	}
}

bool SearchHouseAction::IsDone(const Elite::Blackboard* pBlackboard) const
{
	const SurvivalAgentPlugin* pAgent{ Helpers::GetAgent(pBlackboard) };
	if (pAgent == nullptr)
		return true;

	if (pAgent->IsApproximatelyAt(pAgent->GetDestination()))
		return true;

	const Memory& memory{ pAgent->GetMemory() };

	// Since we can't keep the enemies in memory, we have to ignore them here
	return memory.HasSeenAnyNew(eEntityType::ITEM);
}

Elite::Vector2 SearchHouseAction::GetExploreDestination(const SurvivalAgentPlugin* pAgent, const Elite::Blackboard* pBlackboard) const
{
	HouseInfo targetHouse;
	if (pBlackboard->GetData(TARGET_HOUSE_PARAM, targetHouse))
	{
		return targetHouse.Center;
	}

	return pAgent->GetInterface()->Agent_GetInfo().Position;
}

void SearchHouseAction::SelectTargetHouse(Elite::Blackboard* pBlackboard) const
{
	const SurvivalAgentPlugin* pAgent{ Helpers::GetAgent(pBlackboard) };
	if (pAgent == nullptr)
		return;

	const std::vector<HouseInfo> unsearchedHouses{ pAgent->GetMemory().GetUnsearchedHouses() };
	if (unsearchedHouses.empty())
	{
		std::cout << "No known house to search!\n";
		return;
	}

	const Elite::Vector2 agentLocation{ pAgent->GetInterface()->Agent_GetInfo().Position };
	const HouseInfo closestHouse{ Helpers::GetClosestFromPosition<HouseInfo>(unsearchedHouses, agentLocation, [](const HouseInfo& house) { return house.Center; }) };

	pBlackboard->SetData(TARGET_HOUSE_PARAM, closestHouse);
}
