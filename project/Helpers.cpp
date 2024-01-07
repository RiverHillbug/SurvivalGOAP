#include "stdafx.h"
#include "Helpers.h"
#include "SurvivalAgentPlugin.h"
#include "EliteAI/EliteData/EBlackboard.h"
#include "BlackboardTypes.h"
#include <stdlib.h>
#include <iostream>


bool Helpers::IsNearlyEqual(const float value, const float tolerance /*= 0.01f*/)
{
	return std::abs(value - 100.0f) <= tolerance;
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

void Helpers::ApplyState(const WorldState& stateToApply, OUT Elite::Blackboard* pBlackboard)
{
	for (const auto& state : stateToApply)
	{
		pBlackboard->SetData(state.first, state.second);
	}
}
