#pragma once
#include "FindItemAction.h"

class FindFoodAction : public FindItemAction
{
public:
	FindFoodAction();
	~FindFoodAction() = default;

	eItemType GetItemType() const override { return eItemType::FOOD; }
};
