#include "stdafx.h"
#include "GOAPPlanner.h"
#include "Actions\GOAPAction.h"
#include "EliteAI\EliteData\EBlackboard.h"
#include "BlackboardTypes.h"
#include "SurvivalAgentPlugin.h"
#include "DataProvider.h"
#include "Helpers.h"
#include <EliteMath\EMathUtilities.h>
#include <numeric>

const int Node::InvalidIndex{ -1 };

Node::Node(const int nodeIndex, const int parentNodeIndex, const class GOAPAction* pAction, const WorldState& cumulativeStates, const int cost, const float goalsFulfilledPercentage)
	: m_NodeIndex{ nodeIndex }
	, m_ParentNodeIndex{ parentNodeIndex }
	, m_pAction{ pAction }
	, m_CumulativeStates{ cumulativeStates }
	, m_Cost{ cost }
	, m_GoalsFulfilledPercentage{ goalsFulfilledPercentage }
{

}

void Node::SetData(const int parentNodeIndex, const GOAPAction* pAction, const WorldState& cumulativeStates, const int cost, const float goalsFulfilledPercentage)
{
	m_ParentNodeIndex = parentNodeIndex;
	m_pAction = pAction;
	m_CumulativeStates = cumulativeStates;
	m_Cost = cost;
	m_GoalsFulfilledPercentage = goalsFulfilledPercentage;
}

//----------------------------------------------------------

std::queue<const GOAPAction*> GOAPPlanner::Plan(Elite::Blackboard* pBlackboard)
{
	std::queue<const GOAPAction*> plan;

	SurvivalAgentPlugin* pAgent;
	if (!pBlackboard->GetData(AGENT_PARAM, pAgent))
	{
		// ERROR
		return plan;
	}

	const Goals goals{ pAgent->GetGoals() };
	const WorldState worldState{ DataProvider::GetWorldState(pBlackboard) };

	std::vector<int> successfulNodeIndices{ };


	const float fullGoalsCompletionValue{ GetFullGoalsCompletionValue(goals) };
	const float initialGoalsFulfilledPercentage{ GetGoalsFulfilledPercentage(worldState, goals, fullGoalsCompletionValue) };

	const int startingNodeIndex{ MakeNode(Node::InvalidIndex, nullptr, worldState, 0, initialGoalsFulfilledPercentage) };

	// Try building a graph
	if (!BuildGraph(startingNodeIndex, successfulNodeIndices, pAgent->GetAvailableActions(), goals, fullGoalsCompletionValue))
	{
		m_NextAvailableNodeIndex = 0;
		return plan;
	}

	std::vector<const GOAPAction*> reversePlan;

	int nodeIndex{ GetBestSuccesfulNode(successfulNodeIndices, m_CreatedNodes) };

	while (nodeIndex != startingNodeIndex)
	{
		const Node& node{ m_CreatedNodes[nodeIndex] };
		if (node.m_pAction == nullptr)
		{
			std::cout << "An action in plan was null!\n";
			break;
		}

		reversePlan.push_back(node.m_pAction);
		nodeIndex = node.m_ParentNodeIndex;
	}

	m_NextAvailableNodeIndex = 0;

	// Reverse actions order in plan - std::reverse did not work because it tried to instantiate GOAPActions (which is abstract)?
	for (int i{ int(reversePlan.size()) - 1 }; i >= 0; --i)
	{
		plan.push(reversePlan[i]);
	}

	return plan;
}

bool GOAPPlanner::BuildGraph(const int parentNodeIndex, std::vector<int>& successfulNodeIndices, std::set<const GOAPAction*> availableActions, const Goals& goals, const float fullGoalsCompletionValue)
{
	bool foundSolution{ false };

	for (const auto& action : availableActions)
	{
		if (AreAllPreconditionsMet(action->GetPreconditions(), m_CreatedNodes[parentNodeIndex].m_CumulativeStates))
		{
			const WorldState newState{ ApplyEffects(action, m_CreatedNodes[parentNodeIndex].m_CumulativeStates) };
			const float goalsFulfilledPercentage{ GetGoalsFulfilledPercentage(newState, goals, fullGoalsCompletionValue) };

			const int newNodeIndex{ MakeNode(m_CreatedNodes[parentNodeIndex].m_NodeIndex, action, newState, m_CreatedNodes[parentNodeIndex].m_Cost + action->GetCost(), goalsFulfilledPercentage) };

			if (goalsFulfilledPercentage == 100.0f)
			{
				// Found a complete solution!
				successfulNodeIndices.push_back(newNodeIndex);
				foundSolution = true;
			}
			else
			{
				if (goalsFulfilledPercentage > m_CreatedNodes[parentNodeIndex].m_GoalsFulfilledPercentage)
				{
					// Found a partial solution
					successfulNodeIndices.push_back(newNodeIndex);
					foundSolution = true;
				}

				// No complete solution yet, test for the remaining actions
				const std::set<const GOAPAction*> newAvailableActions{ ActionSubset(availableActions, action) };

				if (!newAvailableActions.empty() && BuildGraph(newNodeIndex, successfulNodeIndices, newAvailableActions, goals, fullGoalsCompletionValue))
				{
					foundSolution = true;
				}
			}
		}
	}

	return foundSolution;
}

bool GOAPPlanner::AreAllPreconditionsMet(const WorldState& preconditions, const WorldState& currentState) const
{
	for (const auto& precondition : preconditions)
	{
		if (currentState.find(precondition.first) != currentState.end())
		{
			if (precondition.second != currentState.at(precondition.first))
			{
				return false;
			}
		}
		// Precondition is not in our current states and needed to be true
		else if (precondition.second == true)
		{
			return false;
		}
	}

	return true;
}


float GOAPPlanner::GetGoalsFulfilledPercentage(const WorldState& currentWorldState, const Goals& goals, const float fullGoalsCompletionValue) const
{
	float goalsFulfilledPercentage{ 0.0f };

	for (const auto& goal : goals)
	{
		const auto& foundState{ currentWorldState.find(goal.first) };

		if (foundState != currentWorldState.end() && foundState->second == true)
		{
			goalsFulfilledPercentage += goal.second / fullGoalsCompletionValue;
		}
	}

	goalsFulfilledPercentage *= 100.0f;

	// Is approximately equal to 100
	const float floatEqualityTolerance{ 0.1f };
	if (std::abs(goalsFulfilledPercentage - 100.0f) <= floatEqualityTolerance)
	{
		goalsFulfilledPercentage = 100.0f;
	}

	return goalsFulfilledPercentage;
}

WorldState GOAPPlanner::ApplyEffects(const GOAPAction* action, const WorldState& currentState)
{
	WorldState newState{ currentState };
	Helpers::ApplyState(action->GetEffects(), newState);
	Helpers::ApplyState(action->GetPlanOnlyEffects(), newState);

	return newState;
}

std::set<const GOAPAction*> GOAPPlanner::ActionSubset(const std::set<const GOAPAction*> actions, const GOAPAction* pActionToExclude)
{
	std::set<const GOAPAction*> actionSubset;

	for (const auto& action : actions)
	{
		if (action != pActionToExclude)
			actionSubset.insert(action);
	}

	return actionSubset;
}

int GOAPPlanner::MakeNode(const int parentNodeIndex, const GOAPAction* pAction, const WorldState& cumulativeStates, const int cost, const float goalsFulfilledPercentage)
{
	const int newNodeIndex{ m_NextAvailableNodeIndex };

	if (newNodeIndex >= int(m_CreatedNodes.size()))
	{
		m_CreatedNodes.emplace_back(newNodeIndex, parentNodeIndex, pAction, cumulativeStates, cost, goalsFulfilledPercentage);
	}
	else
	{
		m_CreatedNodes[newNodeIndex].SetData(parentNodeIndex, pAction, cumulativeStates, cost, goalsFulfilledPercentage);
	}

	++m_NextAvailableNodeIndex;
	return newNodeIndex;
}

float GOAPPlanner::GetFullGoalsCompletionValue(const Goals& goals)
{
	return std::accumulate(goals.begin(), goals.end(), 0.0f,
		[](const float& previousSum, const auto& goal)
		{
			return previousSum + goal.second;
		});
}

int GOAPPlanner::GetBestSuccesfulNode(const std::vector<int>& successfulNodes, const std::vector<Node>& nodes)
{
	// We get the plan that fulfills the most goals with least cost.
	// A plan might fulfill more goals but cost too much and be less preferable than one that completes less goals but is cheap.

	return *std::ranges::max_element(successfulNodes,
		[nodes](int leftIndex, int rightIndex)
		{
			// Goal percentage per cost
			const float leftGoalCompletionPerCost{ nodes[leftIndex].m_GoalsFulfilledPercentage / nodes[leftIndex].m_Cost};
			const float rightGoalCompletionPerCost{ nodes[rightIndex].m_GoalsFulfilledPercentage / nodes[rightIndex].m_Cost };

			return leftGoalCompletionPerCost < rightGoalCompletionPerCost;
		});
}
