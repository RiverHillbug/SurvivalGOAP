#pragma once
#include "GOAPAction.h"

namespace Elite
{
	class Blackboard;
}

class GrabItemAction : public GOAPAction
{
public:
	GrabItemAction();
	~GrabItemAction() = default;

	void Reset() override;
	bool Perform(Elite::Blackboard* pBlackboard) override;
};
