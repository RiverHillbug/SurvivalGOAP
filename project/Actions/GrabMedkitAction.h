#pragma once
#include "GrabItemAction.h"
#include "BlackboardTypes.h"

class GrabMedkitAction : public GrabItemAction
{
public:
	GrabMedkitAction();

	const std::string& GetItemTypeSlotParam() const override { return MEDKIT_SLOTS_PARAM; }
};
