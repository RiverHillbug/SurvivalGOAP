#pragma once
#include "GOAPAction.h"

class FleeFromEnemyAction : public GOAPAction
{
public:
	FleeFromEnemyAction();
	~FleeFromEnemyAction() = default;

	void OnStart(Elite::Blackboard* pBlackboard) const override;
	bool Perform(Elite::Blackboard* pBlackboard) const override;
	void OnExit(Elite::Blackboard* pBlackboard) const override;

	bool IsDone(const Elite::Blackboard* pBlackboard) const override;

private:
	const float m_FleeDistance{ 20.0f };
};
