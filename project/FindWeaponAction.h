#pragma once
#include "GOAPAction.h"

namespace Elite
{
	class Blackboard;
}

class FindWeaponAction : public GOAPAction
{
public:
	FindWeaponAction();
	~FindWeaponAction() = default;
	void Reset() override;
	bool Perform(Elite::Blackboard* pBlackboard) override;
};
