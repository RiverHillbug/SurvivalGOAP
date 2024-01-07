#include "stdafx.h"
#include "GOAPStates.h"
#include "SurvivalAgentPlugin.h"
#include "BlackboardTypes.h"
#include "Actions\GOAPAction.h"
#include "EliteAI\EliteData\EBlackboard.h"
#include "Helpers.h"

void PlanningState::Update(Elite::Blackboard* pBlackboard, float deltaTime) const
{
	SurvivalAgentPlugin* pAgent{ Helpers::GetAgent(pBlackboard) };
	if (pAgent == nullptr)
		return;

	std::queue<const GOAPAction*> plan;

	if (pAgent->TryPlan(plan))
	{
		pAgent->SetPlan(plan);
	}
}

bool PlanningState::IsDone(const Elite::Blackboard* pBlackboard) const
{
	const SurvivalAgentPlugin* pAgent{ Helpers::GetAgent(pBlackboard) };
	return pAgent != nullptr && !pAgent->GetPlan().empty();
}

//----------------------------------------------------------

void MoveToState::Update(Elite::Blackboard* pBlackboard, float deltaTime) const
{
	SurvivalAgentPlugin* pAgent{ Helpers::GetAgent(pBlackboard) };
	if (pAgent == nullptr)
		return;

	const std::queue<const GOAPAction*>& plan{ pAgent->GetPlan() };

	Elite::Vector2 destination;
	if (plan.front() && plan.front()->TryGetDestination(pBlackboard, destination))
	{
		pAgent->SetDestination(destination);
	}
}

bool MoveToState::IsDone(const Elite::Blackboard* pBlackboard) const
{
	const SurvivalAgentPlugin* pAgent{ Helpers::GetAgent(pBlackboard) };
	return pAgent != nullptr && pAgent->IsApproximatelyAt(pAgent->GetDestination());
}

//----------------------------------------------------------

void PerformActionState::Update(Elite::Blackboard* pBlackboard, float deltaTime) const
{
	SurvivalAgentPlugin* pAgent{ Helpers::GetAgent(pBlackboard) };
	if (pAgent == nullptr)
		return;

	std::queue<const GOAPAction*>& plan{ pAgent->GetPlan() };
	const GOAPAction* pCurrentAction{ plan.front() };

	pCurrentAction->Perform(pBlackboard);
}

void PerformActionState::OnExit(Elite::Blackboard* pBlackboard) const
{
	SurvivalAgentPlugin* pAgent{ Helpers::GetAgent(pBlackboard) };
	if (pAgent == nullptr)
		return;

	std::queue<const GOAPAction*>& plan{ pAgent->GetPlan() };

	Helpers::ApplyState(plan.front()->GetEffects(), pBlackboard);
	plan.pop();
}

bool PerformActionState::IsDone(const Elite::Blackboard* pBlackboard) const
{
	const SurvivalAgentPlugin* pAgent{ Helpers::GetAgent(pBlackboard) };
	if (pAgent == nullptr)
		return false;

	const std::queue<const GOAPAction*>& plan{ pAgent->GetPlan() };
	return plan.empty() || plan.front()->IsDone(pBlackboard);
}

//----------------------------------------------------------

bool NeedsRangeCondition::Evaluate(const Elite::Blackboard* pBlackboard) const
{
	const SurvivalAgentPlugin* pAgent{ Helpers::GetAgent(pBlackboard) };
	if (pAgent == nullptr)
		return false;

	const std::queue<const GOAPAction*>& plan{ pAgent->GetPlan() };
	return !plan.empty() && plan.front()->RequiresInRange();
}

//----------------------------------------------------------

bool HasPlanCondition::Evaluate(const Elite::Blackboard* pBlackboard) const
{
	const SurvivalAgentPlugin* pAgent{ Helpers::GetAgent(pBlackboard) };
	return pAgent != nullptr && !pAgent->GetPlan().empty();
}
