#include "stdafx.h"
#include "ExploreAction.h"
#include "EliteAI/EliteData/EBlackboard.h"
#include "SurvivalAgentPlugin.h"
#include "Helpers.h"
#include "BlackboardTypes.h"
#include "IExamInterface.h"
#include <EliteMath/EVector2.h>
#include <EliteMath/EMathUtilities.h>

ExploreAction::ExploreAction()
{
	AddPlanOnlyEffect(EXPLORE_PARAM, true);

	// Very high cost to make sure this action is only used when we can do nothing else
	m_Cost = 10;
}

void ExploreAction::OnStart(Elite::Blackboard* pBlackboard) const
{
	SurvivalAgentPlugin* pAgent{ Helpers::GetAgent(pBlackboard) };
	if (pAgent == nullptr)
		return;

	pBlackboard->SetData<FOVStats>(FOV_STATS_PARAM, pAgent->GetInterface()->FOV_GetStats());

	SendToRandomDestination(pAgent);
	pAgent->SetAngularSpeed(float(E_PI));
	pAgent->SetAutoOrient(false);
}

bool ExploreAction::Perform(Elite::Blackboard* pBlackboard) const
{
	SurvivalAgentPlugin* pAgent{ Helpers::GetAgent(pBlackboard) };
	if (pAgent == nullptr)
		return false;

	if (pAgent->IsApproximatelyAt(pAgent->GetDestination()))
	{
		SendToRandomDestination(pAgent);
	}

	return true;
}

void ExploreAction::OnExit(Elite::Blackboard* pBlackboard) const
{
	GOAPAction::OnExit(pBlackboard);

	SurvivalAgentPlugin* pAgent{ Helpers::GetAgent(pBlackboard) };
	if (pAgent == nullptr)
		return;

	pAgent->SetDestination(pAgent->GetInterface()->Agent_GetInfo().Position);
	pAgent->SetAngularSpeed(0.0f);
	pAgent->SetAutoOrient(true);
}

bool ExploreAction::IsDone(const Elite::Blackboard* pBlackboard) const
{
	const SurvivalAgentPlugin* pAgent{ Helpers::GetAgent(pBlackboard) };
	if (pAgent == nullptr)
		return true;

	FOVStats fovStats;
	if (!pBlackboard->GetData(FOV_STATS_PARAM, fovStats))
	{
		std::cout << "No valid fov stats found in blackboard!\n";
		return true;
	}

	// We stop wandering as soon as we see something new, so we can go back to planning
	return fovStats != pAgent->GetInterface()->FOV_GetStats();
}

void ExploreAction::SendToRandomDestination(class SurvivalAgentPlugin* pAgent) const
{
	const float randomDistanceMax{ 60.0f };

	Elite::Vector2 randomDestination{ Elite::randomVector2(-randomDistanceMax, randomDistanceMax) + pAgent->GetInterface()->Agent_GetInfo().Position };
	pAgent->SetDestination(randomDestination);
}
