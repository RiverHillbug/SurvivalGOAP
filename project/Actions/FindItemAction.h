#pragma once
#include "GOAPAction.h"

namespace Elite
{
	class Blackboard;
}

class FindItemAction : public GOAPAction
{
public:
	FindItemAction();
	~FindItemAction() = default;

	bool Perform(Elite::Blackboard* pBlackboard) const override;
	bool IsDone(const Elite::Blackboard* pBlackboard) const override;

	virtual bool IsCorrectItemType(const struct ItemInfo& item) const = 0;
};
