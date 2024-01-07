#pragma once
#include "UseItemAction.h"
#include "BlackboardTypes.h"

class UseFoodAction : public UseItemAction
{
public:
	UseFoodAction();

	const std::string& GetItemTypeSlotParam() const override { return FOOD_SLOTS_PARAM; }
};
