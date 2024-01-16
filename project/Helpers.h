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
	bool IsNearlyEqual(const float value, const float other, const float tolerance = 0.01f);

	SurvivalAgentPlugin* GetAgent(const Elite::Blackboard* pBlackboard);

	void ApplyState(const WorldState& stateToApply, OUT WorldState& currentState);
	void ApplyState(const WorldState& stateToApply, OUT Elite::Blackboard* pBlackboard);

	bool ShouldConsiderNewPlan(const WorldState& currentState, const WorldState& previousState);

	template<typename T>
	T GetClosestFromPosition(const std::vector<T> items, const Elite::Vector2& position, const std::function<Elite::Vector2(const T&)> positionSelector)
	{
		return *std::ranges::min_element(items,
		[position, positionSelector](const T& left, const T& right)
		{
			const float distanceLeft{ (positionSelector(left) - position).MagnitudeSquared()};
			const float distanceRight{ (positionSelector(right) - position).MagnitudeSquared()};

			return distanceLeft < distanceRight;
		});
	}

	float GetAngleBetweenAgentAndEnemy(const SurvivalAgentPlugin* pAgent, const Elite::Vector2& enemyPosition);

	template<typename T>
	void RemoveEntityFromVector(const int entityHash, std::vector<T>& vector, const std::function<int(const T&)> hashSelector)
	{
		vector.erase(std::ranges::remove_if(vector,
			[entityHash, hashSelector](const T& item)
			{
				return hashSelector(item) == entityHash;
			}).begin(), vector.end());
	}

	void ExcludeSearchedHouses(OUT std::vector<HouseInfo>& houses, const std::unordered_set<HouseInfo>& searchedHouses);
}
