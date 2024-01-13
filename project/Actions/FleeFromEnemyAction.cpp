#include "stdafx.h"
#include "FleeFromEnemyAction.h"
#include "BlackboardTypes.h"
#include "SurvivalAgentPlugin.h"
#include "Helpers.h"
#include <IExamInterface.h>
#include <Exam_HelperStructs.h>

FleeFromEnemyAction::FleeFromEnemyAction()
{
	AddPrecondition(HAS_ENEMY_IN_SIGHT_PARAM, true);
	AddPrecondition(HAS_WEAPON_PARAM, false);

	AddPlanOnlyEffect(FLEE_FROM_ENEMY_PARAM, true);
}

bool FleeFromEnemyAction::Perform(Elite::Blackboard* pBlackboard) const
{
	SurvivalAgentPlugin* pAgent{ Helpers::GetAgent(pBlackboard) };
	if (pAgent == nullptr)
		return false;

	const std::vector<EnemyInfo> enemies{ pAgent->GetInterface()->GetEnemiesInFOV() };
	if (enemies.empty())
		return true;

	const Elite::Vector2 agentLocation{ pAgent->GetInterface()->Agent_GetInfo().Position };
	const EnemyInfo closestEnemy{ Helpers::GetClosestFromPosition<EnemyInfo>(enemies, agentLocation, [](const EnemyInfo& enemy) { return enemy.Location; }) };

	const Elite::Vector2 agentToEnemy{ closestEnemy.Location - agentLocation };
	pAgent->SetDestination(agentToEnemy.GetNormalized() * -m_FleeDistance);

	return true;
}

bool FleeFromEnemyAction::IsDone(const Elite::Blackboard* pBlackboard) const
{
	if (pBlackboard->GetBoolData(HAS_WEAPON_IN_SIGHT_PARAM) ||
		pBlackboard->GetBoolData(HAS_HOUSE_IN_SIGHT_PARAM))
	{
		return true;
	}

	if (const SurvivalAgentPlugin * pAgent{ Helpers::GetAgent(pBlackboard) })
		return pAgent->IsApproximatelyAt(pAgent->GetDestination());

	return true;
}
