#include "stdafx.h"
#include "Helpers.h"
#include "SurvivalAgentPlugin.h"
#include "IExamInterface.h"
#include "EliteAI/EliteData/EBlackboard.h"
#include "BlackboardTypes.h"
#include <stdlib.h>
#include <iostream>

bool Helpers::IsNearlyEqual(const float value, const float other, const float tolerance /*= 0.01f*/)
{
	return std::abs(value - other) <= tolerance;
}

SurvivalAgentPlugin* Helpers::GetAgent(const Elite::Blackboard* pBlackboard)
{
	SurvivalAgentPlugin* pAgent{ nullptr };
	if (!pBlackboard->GetData(AGENT_PARAM, pAgent))
	{
		std::cout << "No valid agent found in blackboard!\n";
	}

	return pAgent;
}

void Helpers::ApplyState(const WorldState& stateToApply, OUT WorldState& currentState)
{
	for (const auto& state : stateToApply)
	{
		if (currentState.find(state.first) != currentState.end())
		{
			currentState[state.first] = state.second;
		}
		else
		{
			currentState.insert({ state.first, state.second });
		}
	}
}

bool Helpers::ShouldConsiderNewPlan(const WorldState& currentState, const WorldState& previousState)
{
	return	(previousState.at(HAS_HIGH_ENERGY_PARAM) && !currentState.at(HAS_HIGH_ENERGY_PARAM)) ||
			(previousState.at(HAS_HIGH_HEALTH_PARAM) && !currentState.at(HAS_HIGH_HEALTH_PARAM));
}

float Helpers::GetAngleBetweenAgentAndEnemy(const SurvivalAgentPlugin* pAgent, const Elite::Vector2& enemyPosition)
{
	const float currentOrientation{ pAgent->GetInterface()->Agent_GetInfo().Orientation };
	const Elite::Vector2 agentToEnemy{ enemyPosition - pAgent->GetInterface()->Agent_GetInfo().Position };
	const float enemyOrientation{ Elite::VectorToOrientation(agentToEnemy) };
	return enemyOrientation - currentOrientation;
}

void Helpers::ExcludeSearchedHouses(OUT std::vector<HouseInfo>& houses, const std::unordered_set<HouseInfo>& searchedHouses)
{
	houses.erase(std::ranges::remove_if(houses,
		[&searchedHouses](const HouseInfo& house)
		{
			return searchedHouses.count(house) != 0;
		}).begin(), houses.end());
}

}

void Helpers::ApplyState(const WorldState& stateToApply, OUT Elite::Blackboard* pBlackboard)
{
	for (const auto& state : stateToApply)
	{
		pBlackboard->SetData(state.first, state.second);
	}
}
