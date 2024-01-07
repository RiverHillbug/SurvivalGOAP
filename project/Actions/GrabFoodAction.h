#pragma once
#include "GrabItemAction.h"
#include "BlackboardTypes.h"

class GrabFoodAction : public GrabItemAction
{
public:
	GrabFoodAction();

	const std::string& GetItemTypeSlotParam() const override { return FOOD_SLOTS_PARAM; }
};
