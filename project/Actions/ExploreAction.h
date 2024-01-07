#pragma once
#include "GOAPAction.h"

namespace Elite
{
	class Blackboard;
}

class ExploreAction : public GOAPAction
{
public:
	ExploreAction();
	~ExploreAction() = default;

	bool Perform(Elite::Blackboard* pBlackboard) const override;
	bool IsDone(const Elite::Blackboard* pBlackboard) const override;
};
