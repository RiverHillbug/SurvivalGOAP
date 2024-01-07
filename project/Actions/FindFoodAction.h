#pragma once
#include "FindItemAction.h"

class FindFoodAction : public FindItemAction
{
public:
	FindFoodAction();
	~FindFoodAction() = default;

	bool IsCorrectItemType(const struct ItemInfo& item) const override;
};
