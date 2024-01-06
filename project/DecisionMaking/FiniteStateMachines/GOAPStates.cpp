#include "stdafx.h"
#include "GOAPStates.h"
#include "SurvivalAgentPlugin.h"
#include "BlackboardTypes.h"
#include "GOAPAction.h"

void PlanningState::Update(Blackboard* pBlackboard, float deltaTime)
{
	SurvivalAgentPlugin* pAgent;
	if (!pBlackboard->GetData(AGENT_PARAM, pAgent))
		return;

	std::queue<const GOAPAction*> plan;

	if (pAgent->TryPlan(plan))
	{
		pAgent->SetPlan(plan);
		m_IsDone = true;
	}
}

void MoveToState::OnEnter(Blackboard* pBlackboard)
{
	FSMState::OnEnter(pBlackboard);
}

void MoveToState::Update(Blackboard* pBlackboard, float deltaTime)
{

}

void PerformActionState::Update(Blackboard* pBlackboard, float deltaTime)
{
	SurvivalAgentPlugin* pAgent;
	if (!pBlackboard->GetData(AGENT_PARAM, pAgent))
		return;

	std::queue<const GOAPAction*> plan{ pAgent->GetPlan() };
	const GOAPAction* pCurrentAction{ plan.front() };

	pCurrentAction->Perform(pBlackboard);
	if (pCurrentAction->IsDone())
	{
		plan.pop();
	}
}

bool NeedsRange::Evaluate(Blackboard* pBlackboard) const
{
	SurvivalAgentPlugin* pAgent;
	if (!pBlackboard->GetData(AGENT_PARAM, pAgent))
		return;

	const std::queue<const GOAPAction*> plan{ pAgent->GetPlan() };

	return !plan.empty() && plan.front()->RequiresInRange();
}
