#pragma once
#include "DataProvider.h"
#include <set>
#include <queue>

namespace Elite
{
	class Blackboard;
}

//Used for building up the graph and holding the costs of actions.
struct Node
{
	Node(const Node* pParent, const class GOAPAction* pAction, const WorldState& cumulativeStates, const int cost, const float goalsFulfilledPercentage);

	void SetData(const Node* pParent, const class GOAPAction* pAction, const WorldState& cumulativeStates, const int cost, const float goalsFulfilledPercentage);

	const Node* m_pParent;
	const class GOAPAction* m_pAction;
	WorldState m_CumulativeStates;	// States added to the plan until this node
	int m_Cost;
	float m_GoalsFulfilledPercentage;
};

class GOAPPlanner
{
public:
	GOAPPlanner() = default;
	~GOAPPlanner() = default;

	std::queue<const class GOAPAction*> Plan(Elite::Blackboard* pBlackboard);

private:
	bool BuildGraph(const Node* pParent, std::vector<const Node*>& successfulNodes, std::set<const GOAPAction*> availableActions, const Goals& goals, const float fullGoalsCompletionValue);
	bool AreAllPreconditionsMet(const WorldState& preconditions, const WorldState& currentState) const;
	float GetGoalsFulfilledPercentage(const WorldState& currentWorldState, const Goals& goals, const float fullGoalsCompletionValue) const;
	WorldState ApplyState(const WorldState& currentStates, const WorldState& statesToApply);
	std::set<const class GOAPAction*> ActionSubset(const std::set<const class GOAPAction*> actions, const class GOAPAction* pActionToExclude);

	const Node* MakeNode(const Node* pParent, const class GOAPAction* pAction, const WorldState& cumulativeStates, const int cost, const float goalsFulfilledPercentage);

	static float GetFullGoalsCompletionValue(const Goals& goals);
	static const Node* GetBestSuccesfulNode(const std::vector<const Node*>& successfulNodes);

	std::vector<Node> m_CreatedNodes;
	int m_NextAvailableNodeIndex{ 0 };
};
