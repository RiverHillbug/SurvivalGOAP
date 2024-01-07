#pragma once
#include "ExploreAction.h"
#include <EliteMath/EVector2.h>

namespace Elite
{
	class Blackboard;
}

class SearchHouseAction : public ExploreAction
{
public:
	SearchHouseAction();

	void OnStart(Elite::Blackboard* pBlackboard) const override;
	bool Perform(Elite::Blackboard* pBlackboard) const override;
	void OnExit(Elite::Blackboard* pBlackboard) const override;

	bool IsDone(const Elite::Blackboard* pBlackboard) const override;

protected:
	Elite::Vector2 GetExploreDestination(const class SurvivalAgentPlugin* pAgent, const Elite::Blackboard* pBlackboard) const override;

private:
	void SelectTargetHouse(Elite::Blackboard* pBlackboard) const;
};
