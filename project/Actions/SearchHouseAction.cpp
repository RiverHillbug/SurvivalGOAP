#include "stdafx.h"
#include "SearchHouseAction.h"
#include "SurvivalAgentPlugin.h"
#include "BlackboardTypes.h"
#include "Helpers.h"
#include <IExamInterface.h>
#include <iostream>
#include <unordered_set>
#include <numeric>

SearchHouseAction::SearchHouseAction()
	: ExploreAction()
{
	AddPrecondition(HAS_HOUSE_IN_SIGHT_PARAM, true);
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

	// Mark the house as searched only if we finished the action through normal means
	if (IsDone(pBlackboard))
	{
		std::unordered_set<HouseInfo> searchedHouses;
		pBlackboard->GetData(SEARCHED_HOUSES_PARAM, searchedHouses);

		searchedHouses.insert(targetHouse);
		pBlackboard->SetData(SEARCHED_HOUSES_PARAM, searchedHouses);
	}

	pBlackboard->RemoveData(TARGET_HOUSE_PARAM);
}

bool SearchHouseAction::IsDone(const Elite::Blackboard* pBlackboard) const
{
	if (ExploreAction::IsDone(pBlackboard))
		return true;

	const SurvivalAgentPlugin* pAgent{ Helpers::GetAgent(pBlackboard) };
	return pAgent == nullptr || pAgent->IsApproximatelyAt(pAgent->GetDestination());
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

	std::vector<HouseInfo> housesInFOV{ pAgent->GetInterface()->GetHousesInFOV() };

	if (housesInFOV.empty())
	{
		std::cout << "No houses in fov!\n";
		return;
	}

	std::unordered_set<HouseInfo> searchedHouses;
	pBlackboard->GetData(SEARCHED_HOUSES_PARAM, searchedHouses);

	Helpers::ExcludeSearchedHouses(housesInFOV, searchedHouses);

	if (housesInFOV.empty())
		return;

	const Elite::Vector2 agentLocation{ pAgent->GetInterface()->Agent_GetInfo().Position };
	const HouseInfo closestHouse{ Helpers::GetClosestFromPosition<HouseInfo>(housesInFOV, agentLocation, [](const HouseInfo& house) { return house.Center; }) };

	pBlackboard->SetData(TARGET_HOUSE_PARAM, closestHouse);
}
