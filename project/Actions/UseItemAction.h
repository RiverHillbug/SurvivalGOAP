#pragma once
#include "GOAPAction.h"

namespace Elite
{
	class Blackboard;
}

class UseItemAction : public GOAPAction
{
public:
	UseItemAction();
	~UseItemAction() = default;

	bool Perform(Elite::Blackboard* pBlackboard) const override;
};
