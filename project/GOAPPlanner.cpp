#include "stdafx.h"
#include "GOAPPlanner.h"
#include "Actions\GOAPAction.h"
#include "EliteAI\EliteData\EBlackboard.h"
#include "BlackboardTypes.h"
#include "SurvivalAgentPlugin.h"
#include "DataProvider.h"
#include <EliteMath\EMathUtilities.h>
#include <numeric>

Node::Node(const Node* pParent, const class GOAPAction* pAction, const WorldState& cumulativeStates, const int cost, const float goalsFulfilledPercentage)
	: m_pParent{ pParent }
	, m_pAction{ pAction }
	, m_CumulativeStates{ cumulativeStates }
	, m_Cost{ cost }
	, m_GoalsFulfilledPercentage{ goalsFulfilledPercentage }
{

}

void Node::SetData(const Node* pParent, const GOAPAction* pAction, const WorldState& cumulativeStates, const int cost, const float goalsFulfilledPercentage)
{
	m_pParent = pParent;
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

	std::vector<const Node*> succesfulNodes{ };


	const float fullGoalsCompletionValue{ GetFullGoalsCompletionValue(goals) };
	const float initialGoalsFulfilledPercentage{ GetGoalsFulfilledPercentage(worldState, goals, fullGoalsCompletionValue) };

	const Node pStartingNode{ nullptr, nullptr, worldState, 0, initialGoalsFulfilledPercentage };

	// Try building a graph
	if (!BuildGraph(&pStartingNode, succesfulNodes, pAgent->GetAvailableActions(), goals, fullGoalsCompletionValue))
	{
		m_NextAvailableNodeIndex = 0;

		std::cout << "No plan!\n";
		return plan;
	}

	std::vector<const GOAPAction*> reversePlan;

	const Node* pNode{ GetBestSuccesfulNode(succesfulNodes) };
	while (pNode != nullptr)
	{
		if (pNode->m_pAction == nullptr)
			break;

		reversePlan.push_back(pNode->m_pAction);
		pNode = pNode->m_pParent;
	}

	m_NextAvailableNodeIndex = 0;

	// Reverse actions order in plan - std::reverse did not work because it tried to instantiate GOAPActions (which is abstract)?
	for (int i{ int(reversePlan.size()) - 1 }; i >= 0; --i)
	{
		plan.push(reversePlan[i]);
	}

	return plan;
}

bool GOAPPlanner::BuildGraph(const Node* pParent, std::vector<const Node*>& successfulNodes, std::set<const GOAPAction*> availableActions, const Goals& goals, const float fullGoalsCompletionValue)
{
	bool foundSolution{ false };

	for (const auto& action : availableActions)
	{
		if (AreAllPreconditionsMet(action->GetPreconditions(), pParent->m_CumulativeStates))
		{
			const WorldState newState{ ApplyState(pParent->m_CumulativeStates, action->GetEffects()) };
			const float goalsFulfilledPercentage{ GetGoalsFulfilledPercentage(newState, goals, fullGoalsCompletionValue) };

			const Node* pNewNode{ MakeNode(pParent, action, newState, pParent->m_Cost + action->GetCost(), goalsFulfilledPercentage) };

			if (goalsFulfilledPercentage == 100.0f)
			{
				// Found a complete solution!
				successfulNodes.push_back(pNewNode);
				foundSolution = true;
			}
			else
			{
				if (goalsFulfilledPercentage > pParent->m_GoalsFulfilledPercentage)
				{
					// Found a partial solution
					successfulNodes.push_back(pNewNode);
					foundSolution = true;
				}

				// No complete solution yet, test for the remaining actions
				const std::set<const GOAPAction*> newAvailableActions{ ActionSubset(availableActions, action) };

				if (!newAvailableActions.empty() && BuildGraph(pNewNode, successfulNodes, newAvailableActions, goals, fullGoalsCompletionValue))
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

WorldState GOAPPlanner::ApplyState(const WorldState& currentStates, const WorldState& statesToApply)
{
	WorldState newStates{ currentStates };

	for (const auto& state : statesToApply)
	{
		if (newStates.find(state.first) != newStates.end())
		{
			newStates[state.first] = state.second;
		}
		else
		{
			newStates.insert({ state.first, state.second });
		}
	}

	return newStates;
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

const Node* GOAPPlanner::MakeNode(const Node* pParent, const GOAPAction* pAction, const WorldState& cumulativeStates, const int cost, const float goalsFulfilledPercentage)
{
	const Node* pNode{ nullptr };

	if (m_NextAvailableNodeIndex < int(m_CreatedNodes.size()))
	{
		m_CreatedNodes[m_NextAvailableNodeIndex].SetData(pParent, pAction, cumulativeStates, cost, goalsFulfilledPercentage);
		pNode = &m_CreatedNodes[m_NextAvailableNodeIndex];
	}
	else
	{
		pNode = &m_CreatedNodes.emplace_back(pParent, pAction, cumulativeStates, cost, goalsFulfilledPercentage);
	}

	++m_NextAvailableNodeIndex;
	return pNode;
}

float GOAPPlanner::GetFullGoalsCompletionValue(const Goals& goals)
{
	return std::accumulate(goals.begin(), goals.end(), 0.0f,
		[](const float& previousSum, const auto& goal)
		{
			return previousSum + goal.second;
		});
}

const Node* GOAPPlanner::GetBestSuccesfulNode(const std::vector<const Node*>& successfulNodes)
{
	// We get the plan that fulfills the most goals with least cost.
	// A plan might fulfill more goals but cost too much and be less preferable than one that completes less goals but is cheap.

	return *std::ranges::max_element(successfulNodes,
		[](const Node* left, const Node* right)
		{
			// Goal percentage per cost
			const float leftGoalCompletionPerCost{ left->m_GoalsFulfilledPercentage / left->m_Cost };
			const float rightGoalCompletionPerCost{ right->m_GoalsFulfilledPercentage / right->m_Cost };

			return leftGoalCompletionPerCost < rightGoalCompletionPerCost;
		});
}
