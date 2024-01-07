#pragma once
#include "DataProvider.h"
#include <unordered_map>
#include <EliteMath/EVector2.h>

namespace Elite
{
	class Blackboard;
}

class GOAPAction
{
public:
	GOAPAction() = default;
	~GOAPAction() = default;

	inline const WorldState& GetPreconditions() const { return m_Preconditions; }
	inline const WorldState& GetEffects() const { return m_Effects; }
	inline const WorldState& GetPlanOnlyEffects() const { return m_PlanOnlyEffects; }

	inline int GetCost() const { return m_Cost; }
	inline bool GetInRange() const { return m_InRange; }
	inline void SetInRange(bool inRange) { m_InRange = inRange; }
	inline bool RequiresInRange() const { return m_RequiresInRange; }
	virtual bool TryGetDestination(const Elite::Blackboard* pBlackboard, OUT Elite::Vector2& destination) const { return false; }

	virtual bool Perform(Elite::Blackboard* pBlackboard) const = 0;
	virtual bool IsDone(const Elite::Blackboard* pBlackboard) const = 0;

protected:
	int m_Cost{ 1 };
	bool m_InRange{ false };
	bool m_RequiresInRange{ false };

	void AddPrecondition(const std::string& name, const bool value);
	void AddEffect(const std::string& name, const bool value);
	void AddPlanOnlyEffect(const std::string& name, const bool value);

private:
	WorldState m_Preconditions{};
	WorldState m_Effects{};

	// Effects used only for planning, that should not be added to the blackboard on completion
	WorldState m_PlanOnlyEffects{};

	static void AddToWorldState(const std::string& name, const bool value, OUT WorldState& worldState);
};
