#pragma once
#include "DataProvider.h"
#include <set>
#include <unordered_map>

namespace Elite
{
	class Blackboard;
}

class GOAPPlanner
{
public:
	GOAPPlanner() = default;
	~GOAPPlanner() = default;

	std::queue<const class GOAPAction*> Plan(Elite::Blackboard* pBlackboard);

private:
	bool BuildGraph(const struct Node* pParent, Node*& cheapestSuccesfulNode, const WorldState& goal, std::set<const GOAPAction*> availableActions, int& currentLowestPlanCost);
	bool AreAllPreconditionsMet(const WorldState& preconditions, const WorldState& currentState) const;
	WorldState ApplyState(const WorldState& currentStates, const WorldState& statesToApply);
	std::set<const class GOAPAction*> ActionSubset(const std::set<const class GOAPAction*> actions, const class GOAPAction* pActionToExclude);
};

//Used for building up the graph and holding the costs of actions.
struct Node
{
	Node(const Node* pParent, const WorldState& cumulativeStates, int cost, const class GOAPAction* pAction)
		: m_pParent{ pParent }
		, m_CumulativeStates{ cumulativeStates }
		, m_Cost{ cost }
		, m_pAction{ pAction }
	{
	}

	const Node* m_pParent;
	WorldState m_CumulativeStates;	// States added to the plan until this node
	int m_Cost;
	const class GOAPAction* m_pAction;
};
