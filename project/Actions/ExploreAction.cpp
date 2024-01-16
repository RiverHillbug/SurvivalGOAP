#include "stdafx.h"
#include "ExploreAction.h"
#include "EliteAI/EliteData/EBlackboard.h"
#include "SurvivalAgentPlugin.h"
#include "Helpers.h"
#include "BlackboardTypes.h"
#include "IExamInterface.h"
#include <EliteMath/EVector2.h>
#include <EliteMath/EMathUtilities.h>

ExploreAction::ExploreAction(const WorldInfo& worldInfo)
{
	AddPlanOnlyEffect(EXPLORE_PARAM, true);

	// Very high cost to make sure this action is only used when we can do nothing else
	m_Cost = 10;

	m_WorldCenter = worldInfo.Center;
	m_WorldDimensions = worldInfo.Dimensions;
}

void ExploreAction::OnStart(Elite::Blackboard* pBlackboard) const
{
	SurvivalAgentPlugin* pAgent{ Helpers::GetAgent(pBlackboard) };
	if (pAgent == nullptr)
		return;

	pAgent->SetDestination(GetExploreDestination(pAgent, pBlackboard));
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
		pAgent->SetDestination(GetExploreDestination(pAgent, pBlackboard));
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

	if (pAgent->IsApproximatelyAt(pAgent->GetDestination()))
		return true;

	const Memory& memory{ pAgent->GetMemory() };
	return	memory.HasSeenAnyNew(eEntityType::ENEMY) ||
			memory.HasSeenAnyNew(eEntityType::ITEM) ||
			memory.HasSeenAnyNew(eEntityType::HOUSE);
}

Elite::Vector2 ExploreAction::GetExploreDestination(const SurvivalAgentPlugin* pAgent, const Elite::Blackboard* pBlackboard) const
{
	const float randomDistanceMax{ 60.0f };
	Elite::Vector2 destination;

	const float top{ m_WorldCenter.y + (m_WorldDimensions.y * 0.5f) };
	const float left{ m_WorldCenter.x - (m_WorldDimensions.x * 0.5f) };
	const float bottom{ m_WorldCenter.y - (m_WorldDimensions.y * 0.5f) };
	const float right{ m_WorldCenter.x + (m_WorldDimensions.x * 0.5f) };

	// Stay within the world borders
	do
	{
		destination = Elite::randomVector2(-randomDistanceMax, randomDistanceMax) + pAgent->GetInterface()->Agent_GetInfo().Position;
	} while (destination.x >= right || destination.x <= left ||
			 destination.y >= top || destination.y <= bottom);

	return destination;
}
