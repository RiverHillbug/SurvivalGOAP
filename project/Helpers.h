#pragma once
#include "DataProvider.h"

namespace Elite
{
	class Blackboard;
}

class SurvivalAgentPlugin;

namespace Helpers
{
	bool IsNearlyEqual(const float value, const float tolerance = 0.01f);

	SurvivalAgentPlugin* GetAgent(const Elite::Blackboard* pBlackboard);

	void ApplyState(const WorldState& stateToApply, OUT WorldState& currentState);
	void ApplyState(const WorldState& stateToApply, OUT Elite::Blackboard* pBlackboard);
}
