#pragma once
#include "DataProvider.h"
#include <unordered_map>

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
	inline int GetCost() const { return m_Cost; }
	inline bool GetInRange() const { return m_InRange; }
	inline void SetInRange(bool inRange) { m_InRange = inRange; }
	inline bool RequiresInRange() const { return m_RequiresInRange; }
	inline bool IsDone() const { return m_IsDone; }
	virtual void Reset();
	virtual bool Perform(Elite::Blackboard* pBlackboard) = 0;

protected:
	int m_Cost{ 1 };
	bool m_IsDone{ false };
	bool m_InRange{ false };
	bool m_RequiresInRange{ false };
	WorldState m_Preconditions{};
	WorldState m_Effects{};

	void AddPrecondition(const std::string& name, const bool value);
	void AddEffect(const std::string& name, const bool value);
};
