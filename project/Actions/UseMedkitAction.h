#pragma once
#include "UseItemAction.h"
#include "BlackboardTypes.h"

class UseMedkitAction : public UseItemAction
{
public:
	UseMedkitAction();

	const std::string& GetItemTypeSlotParam() const override { return MEDKIT_SLOTS_PARAM; }
};
