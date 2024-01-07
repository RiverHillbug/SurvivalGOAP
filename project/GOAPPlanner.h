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
	Node(const int nodeIndex, const int parentNodeIndex, const class GOAPAction* pAction, const WorldState& cumulativeStates, const int cost, const float goalsFulfilledPercentage);

	void SetData(const int parentNodeIndex, const class GOAPAction* pAction, const WorldState& cumulativeStates, const int cost, const float goalsFulfilledPercentage);

	int m_NodeIndex;
	int m_ParentNodeIndex;
	const class GOAPAction* m_pAction;
	WorldState m_CumulativeStates;	// States added to the plan until this node
	int m_Cost;
	float m_GoalsFulfilledPercentage;

	static const int InvalidIndex;
};

class GOAPPlanner
{
public:
	GOAPPlanner() = default;
	~GOAPPlanner() = default;

	std::queue<const class GOAPAction*> Plan(Elite::Blackboard* pBlackboard);

private:
	bool BuildGraph(const int parentNodeIndex, std::vector<int>& successfulNodeIndices, std::set<const GOAPAction*> availableActions, const Goals& goals, const float fullGoalsCompletionValue);
	bool AreAllPreconditionsMet(const WorldState& preconditions, const WorldState& currentState) const;
	float GetGoalsFulfilledPercentage(const WorldState& currentWorldState, const Goals& goals, const float fullGoalsCompletionValue) const;
	WorldState ApplyEffects(const class GOAPAction* action, const WorldState& currentState);
	std::set<const class GOAPAction*> ActionSubset(const std::set<const class GOAPAction*> actions, const class GOAPAction* pActionToExclude);

	// Adding a new item in the nodes vector invalidates previous pointers to its elements; we therefore have to work with indices instead of pointers if we want to avoid instantiating nodes on the heap
	int MakeNode(const int parentNodeIndex, const class GOAPAction* pAction, const WorldState& cumulativeStates, const int cost, const float goalsFulfilledPercentage);

	static float GetFullGoalsCompletionValue(const Goals& goals);
	static int GetBestSuccesfulNode(const std::vector<int>& successfulNodeIndices, const std::vector<Node>& nodes);

	std::vector<Node> m_CreatedNodes;
	int m_NextAvailableNodeIndex{ 0 };
};
