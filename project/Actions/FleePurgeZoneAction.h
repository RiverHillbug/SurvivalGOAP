#pragma once
#include "GOAPAction.h"

class FleePurgeZoneAction : public GOAPAction
{
public:
	FleePurgeZoneAction();
	~FleePurgeZoneAction() = default;

	void OnStart(Elite::Blackboard* pBlackboard) const override;
	bool Perform(Elite::Blackboard* pBlackboard) const override;
	void OnExit(Elite::Blackboard* pBlackboard) const override;

	bool IsDone(const Elite::Blackboard* pBlackboard) const override;
};
