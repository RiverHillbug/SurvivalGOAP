#pragma once
#include "GrabItemAction.h"
#include "BlackboardTypes.h"

class DestroyGarbageAction : public GrabItemAction
{
public:
	DestroyGarbageAction();

	const std::string& GetItemTypeSlotParam() const override { return NO_SLOTS_PARAM; }

protected:
	bool GrabItem(class SurvivalAgentPlugin* pAgent, const UINT slot, const struct ItemInfo& item) const override;
};
