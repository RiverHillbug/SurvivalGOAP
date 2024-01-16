#pragma once
#include "UseItemAction.h"
#include "BlackboardTypes.h"

class UseWeaponAction : public UseItemAction
{
public:
	UseWeaponAction();

	void OnStart(Elite::Blackboard* pBlackboard) const override;
	void OnExit(Elite::Blackboard* pBlackboard) const override;

	const std::string& GetItemTypeSlotParam() const override { return WEAPON_SLOTS_PARAM; }

	void FaceTarget(class SurvivalAgentPlugin* pAgent, Elite::Blackboard* pBlackboard) const override;
	bool HasTarget(const class SurvivalAgentPlugin* pAgent, const Elite::Blackboard* pBlackboard) const override;
	bool CanUseItem(const class SurvivalAgentPlugin* pAgent, const Elite::Blackboard* pBlackboard) const override;

	void OnAfterUseItem(class SurvivalAgentPlugin* pAgent, Elite::Blackboard* pBlackboard) const override;

private:
	const float m_FacingAngleRadians{ 2.0f * (float(E_PI) / 180.0f) };
	const float m_FleeDistance{ 10.0f };
};
