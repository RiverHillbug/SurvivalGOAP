#include "stdafx.h"
#include "GOAPPlanner.h"
#include "GOAPAction.h"
#include "EliteAI\EliteData\EBlackboard.h"
#include "BlackboardTypes.h"
#include "SurvivalAgentPlugin.h"
#include "DataProvider.h"
#include <EliteMath\EMathUtilities.h>

std::queue<const GOAPAction*> GOAPPlanner::Plan(Elite::Blackboard* pBlackboard)
{
	std::queue<const GOAPAction*> plan;

	SurvivalAgentPlugin* pAgent;
	if (!pBlackboard->GetData(AGENT_PARAM, pAgent))
	{
		// ERROR
		return plan;
	}

	const WorldState goal{ pAgent->GetGoals() };
	const WorldState worldState{ DataProvider::GetWorldState(pBlackboard) };

	std::vector<const Node*> succesfulNodes{ };
	Node* pCheapestSuccesfulNode{ nullptr };

	const Node* pStartingNode{ new Node(nullptr, worldState, 0, nullptr) };

	// Try building a graph
	int lowestPlanCost{ INT_MAX };

	if (!BuildGraph(pStartingNode, pCheapestSuccesfulNode, goal, pAgent->GetAvailableActions(), lowestPlanCost))
	{
		std::cout << "No plan!\n";
		return plan;
	}

	std::vector<const GOAPAction*> reversePlan;

	const Node* pNode{ pCheapestSuccesfulNode };
	while (pNode != nullptr)
	{
		if (pNode->m_pAction == nullptr)
			break;

		reversePlan.push_back(pNode->m_pAction);
		pNode = pNode->m_pParent;
	}

	// Reverse actions order in plan - std::reverse did not work because it tried to instantiate GOAPActions (abstract)?
	for (int i{ int(reversePlan.size()) - 1 }; i >= 0; --i)
	{
		plan.push(reversePlan[i]);
	}

	return plan;
}

bool GOAPPlanner::BuildGraph(const Node* pParent, Node*& pCheapestSuccesfulNode, const WorldState& goal, std::set<const GOAPAction*> availableActions, int& currentLowestPlanCost)
{
	bool foundSolution{ false };

	for (const auto& action : availableActions)
	{
		if (AreAllPreconditionsMet(action->GetPreconditions(), pParent->m_CumulativeStates))
		{
			const WorldState newState{ ApplyState(pParent->m_CumulativeStates, action->GetEffects()) };
			const int newCost{ (pParent->m_Cost + action->GetCost()) };
			if (newCost > currentLowestPlanCost)
				continue;

			Node* pNewNode{ new Node(pParent, newState, newCost, action) };

			if (AreAllPreconditionsMet(goal, newState))
			{
				// Found a solution!
				pCheapestSuccesfulNode = pNewNode;
				currentLowestPlanCost = newCost;
				return true;
			}
			else
			{
				// No solution yet, test for the remaining actions
				const std::set<const GOAPAction*> newAvailableActions{ ActionSubset(availableActions, action) };
				if (BuildGraph(pNewNode, pCheapestSuccesfulNode, goal, newAvailableActions, currentLowestPlanCost))
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
