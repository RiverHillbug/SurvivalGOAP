#pragma once
#include "GOAPAction.h"

namespace Elite
{
	class Blackboard;
}

class UseItemAction : public GOAPAction
{
public:
	UseItemAction();
	~UseItemAction() = default;

	bool Perform(Elite::Blackboard* pBlackboard) const override;
	void OnExit(Elite::Blackboard* pBlackboard) const override;

	bool IsDone(const Elite::Blackboard* pBlackboard) const override;

	virtual const std::string& GetItemTypeSlotParam() const = 0;

	virtual void FaceTarget(class SurvivalAgentPlugin* pAgent, Elite::Blackboard* pBlackboard) const {}
	virtual bool HasTarget(const class SurvivalAgentPlugin* pAgent, const Elite::Blackboard* pBlackboard) const { return false; }
	virtual bool CanUseItem(const class SurvivalAgentPlugin* pAgent, const Elite::Blackboard* pBlackboard) const { return true; }

	virtual void OnAfterUseItem(class SurvivalAgentPlugin* pAgent, Elite::Blackboard* pBlackboard) const {}

protected:
	bool m_NeedsToFaceTarget{ false };
};
