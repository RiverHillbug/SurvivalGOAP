#pragma once
#include "GOAPAction.h"
#include <Exam_HelperStructs.h>

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

	virtual eItemType GetItemType() const = 0;
};
