#pragma once
#include "UseItemAction.h"
#include "BlackboardTypes.h"

class UseWeaponAction : public UseItemAction
{
public:
	UseWeaponAction();

	const std::string& GetItemTypeSlotParam() const override { return WEAPON_SLOTS_PARAM; }

	void FaceTarget(class SurvivalAgentPlugin* pAgent, Elite::Blackboard* pBlackboard) const override;
	bool HasTarget(const class SurvivalAgentPlugin* pAgent, const Elite::Blackboard* pBlackboard) const override;
};
