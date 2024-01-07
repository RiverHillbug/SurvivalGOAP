#pragma once
#include "DataProvider.h"
#include <EliteMath/EVector2.h>
#include <Exam_HelperStructs.h>
#include <functional>
#include <unordered_set>

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

	template<typename T>
	T GetClosestFromPosition(const std::vector<T> items, const Elite::Vector2& position, std::function<Elite::Vector2(const T&)> positionSelector)
	{
		return *std::ranges::min_element(items,
		[position, positionSelector](const T& left, const T& right)
		{
			const float distanceLeft{ (positionSelector(left) - position).MagnitudeSquared()};
			const float distanceRight{ (positionSelector(right) - position).MagnitudeSquared()};

			return distanceLeft < distanceRight;
		});
	}

	void ExcludeSearchedHouses(OUT std::vector<HouseInfo>& houses, const std::unordered_set<HouseInfo>& searchedHouses);
}
