#include "stdafx.h"
#include "FleeFromEnemyAction.h"
#include "BlackboardTypes.h"
#include "SurvivalAgentPlugin.h"
#include "Helpers.h"
#include <IExamInterface.h>
#include <Exam_HelperStructs.h>

FleeFromEnemyAction::FleeFromEnemyAction()
{
	AddPrecondition(HAS_ENEMY_IN_MEMORY_PARAM, true);
	AddPrecondition(HAS_WEAPON_PARAM, false);

	AddPlanOnlyEffect(FLEE_FROM_ENEMY_PARAM, true);

	m_Cost = 8;
}

void FleeFromEnemyAction::OnStart(Elite::Blackboard* pBlackboard) const
{
	if (SurvivalAgentPlugin* pAgent{ Helpers::GetAgent(pBlackboard) })
	{
		pAgent->SetAngularSpeed(float(E_PI));
		pAgent->SetAutoOrient(false);
		pAgent->SetDestination(pAgent->GetInterface()->Agent_GetInfo().Position);
	}
}

bool FleeFromEnemyAction::Perform(Elite::Blackboard* pBlackboard) const
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

	if (!pAgent->IsApproximatelyAt(pAgent->GetDestination()) && !pAgent->GetMemory().HasSeenAnyNew(eEntityType::ENEMY))
		return true;

	const std::vector<EnemyInfo> enemies{ pAgent->GetMemory().GetEnemiesInMemory() };
	if (enemies.empty())
		return true;

	const Elite::Vector2 agentLocation{ pAgent->GetInterface()->Agent_GetInfo().Position };
	const EnemyInfo closestEnemy{ Helpers::GetClosestFromPosition<EnemyInfo>(enemies, agentLocation, [](const EnemyInfo& enemy) { return enemy.Location; }) };

	const Elite::Vector2 agentToEnemy{ closestEnemy.Location - agentLocation };
	const Elite::Vector2 fleeDestination{ agentToEnemy.GetNormalized() * -m_FleeDistance };
	pAgent->SetDestination(agentLocation + fleeDestination);

	return true;
}

bool FleeFromEnemyAction::IsDone(const Elite::Blackboard* pBlackboard) const
{
	if (pBlackboard->GetBoolData(HAS_WEAPON_IN_MEMORY_PARAM) ||
		!pBlackboard->GetBoolData(HAS_ENEMY_IN_MEMORY_PARAM))
		return true;

	const SurvivalAgentPlugin* pAgent{ Helpers::GetAgent(pBlackboard) };
	if (pAgent == nullptr)
		return true;

	return pAgent->GetMemory().HasSeenAnyNew(eEntityType::HOUSE);
}

void FleeFromEnemyAction::OnExit(Elite::Blackboard* pBlackboard) const
{
	if (SurvivalAgentPlugin* pAgent{ Helpers::GetAgent(pBlackboard) })
	{
		pAgent->SetCanRun(false);
		pAgent->SetAutoOrient(true);
	}
}
