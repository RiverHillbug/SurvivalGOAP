#pragma once
#include "GOAPAction.h"
#include <EliteMath/EVector2.h>

namespace Elite
{
	class Blackboard;
}

class GrabItemAction : public GOAPAction
{
public:
	GrabItemAction();
	~GrabItemAction() = default;

	bool TryGetDestination(const Elite::Blackboard* pBlackboard, OUT Elite::Vector2& destination) const override;

	bool Perform(Elite::Blackboard* pBlackboard) const override;
	bool IsDone(const Elite::Blackboard* pBlackboard) const override;

	virtual const std::string& GetItemTypeSlotParam() const = 0;

protected:
	virtual bool GrabItem(SurvivalAgentPlugin* pAgent, const UINT slot, const struct ItemInfo& item) const;

private:
	void FaceTarget(class SurvivalAgentPlugin* pAgent, Elite::Blackboard* pBlackboard) const;
};
