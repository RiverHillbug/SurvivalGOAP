#include "stdafx.h"
#include "FleePurgeZoneAction.h"
#include "BlackboardTypes.h"
#include "SurvivalAgentPlugin.h"
#include "Helpers.h"
#include <IExamInterface.h>
#include <Exam_HelperStructs.h>

FleePurgeZoneAction::FleePurgeZoneAction()
	: GOAPAction()
{
	AddPrecondition(HAS_PURGE_ZONE_IN_MEMORY_PARAM, true);

	AddPlanOnlyEffect(IS_NOT_IN_PURGE_ZONE_PARAM, true);

	m_Cost = 1;
}

void FleePurgeZoneAction::OnStart(Elite::Blackboard* pBlackboard) const
{
	if (SurvivalAgentPlugin* pAgent{ Helpers::GetAgent(pBlackboard) })
	{
		pAgent->SetAngularSpeed(float(E_PI));
		pAgent->SetAutoOrient(false);
	}
}

bool FleePurgeZoneAction::Perform(Elite::Blackboard* pBlackboard) const
{
	SurvivalAgentPlugin* pAgent{ Helpers::GetAgent(pBlackboard) };
	if (pAgent == nullptr)
		return false;

	if (pBlackboard->GetBoolData(HAS_HIGH_STAMINA_PARAM))
	{
		pAgent->SetCanRun(true);
	}
	if (pBlackboard->GetBoolData(HAS_LOW_STAMINA_PARAM))
	{
		pAgent->SetCanRun(false);
	}

	const std::vector<PurgeZoneInfo> purgeZones{ };
	if (purgeZones.empty())
		return true;

	const Elite::Vector2 agentLocation{ pAgent->GetInterface()->Agent_GetInfo().Position };
	const PurgeZoneInfo closestPurgeZone{ Helpers::GetClosestFromPosition<PurgeZoneInfo>(purgeZones, agentLocation, [](const PurgeZoneInfo& purgeZone) { return purgeZone.Center; }) };

	const Elite::Vector2 agentToPurgeZone{ closestPurgeZone.Center - agentLocation };
	const Elite::Vector2 fleeDestination{ agentToPurgeZone.GetNormalized() * -(closestPurgeZone.Radius + 10.0f) };
	pAgent->SetDestination(agentLocation + fleeDestination);

	return true;
}

void FleePurgeZoneAction::OnExit(Elite::Blackboard* pBlackboard) const
{
	if (SurvivalAgentPlugin * pAgent{ Helpers::GetAgent(pBlackboard) })
	{
		pAgent->SetCanRun(false);
		pAgent->SetAutoOrient(true);
	}
}

bool FleePurgeZoneAction::IsDone(const Elite::Blackboard* pBlackboard) const
{
	return !Helpers::IsAgentInPurgeZone(pBlackboard) && Helpers::GetAgent(pBlackboard)->GetMemory().GetPurgeZonesInMemory().empty();
}
