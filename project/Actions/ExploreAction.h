#pragma once
#include "GOAPAction.h"

namespace Elite
{
	class Blackboard;
}

class ExploreAction : public GOAPAction
{
public:
	ExploreAction(const struct WorldInfo& worldInfo);
	~ExploreAction() = default;

	void OnStart(Elite::Blackboard* pBlackboard) const override;
	bool Perform(Elite::Blackboard* pBlackboard) const override;
	void OnExit(Elite::Blackboard* pBlackboard) const override;

	bool IsDone(const Elite::Blackboard* pBlackboard) const override;

protected:
	virtual Elite::Vector2 GetExploreDestination(const class SurvivalAgentPlugin* pAgent, const Elite::Blackboard* pBlackboard) const;

private:
	Elite::Vector2 m_WorldCenter;
	Elite::Vector2 m_WorldDimensions;
};
