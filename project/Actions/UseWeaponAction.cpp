#include "stdafx.h"
#include "UseWeaponAction.h"
#include "BlackboardTypes.h"
#include "SurvivalAgentPlugin.h"
#include "EliteAI/EliteData/EBlackboard.h"
#include "Exam_HelperStructs.h"
#include "IExamInterface.h"
#include "Helpers.h"
#include <vector>
#include <numeric>

UseWeaponAction::UseWeaponAction()
	: UseItemAction()
{
	AddPrecondition(HAS_WEAPON_PARAM, true);
	AddPrecondition(HAS_ENEMY_IN_MEMORY_PARAM, true);

	AddPlanOnlyEffect(KILL_ENEMY_PARAM, true);

	m_NeedsToFaceTarget = true;
}

void UseWeaponAction::OnStart(Elite::Blackboard* pBlackboard) const
{
	if (SurvivalAgentPlugin * pAgent{ Helpers::GetAgent(pBlackboard) })
	{
		pAgent->SetAutoOrient(false);
	}
}

void UseWeaponAction::OnExit(Elite::Blackboard* pBlackboard) const
{
	UseItemAction::OnExit(pBlackboard);

	if (SurvivalAgentPlugin * pAgent{ Helpers::GetAgent(pBlackboard) })
	{
		pAgent->SetAutoOrient(true);
	}
}

void UseWeaponAction::FaceTarget(SurvivalAgentPlugin* pAgent, Elite::Blackboard* pBlackboard) const
{
	std::vector<EnemyInfo> enemies{ pAgent->GetInterface()->GetEnemiesInFOV() };
	if (enemies.empty())
		enemies = pAgent->GetMemory().GetEnemiesInMemory();

	if (enemies.empty())
		return;

	const Elite::Vector2 agentLocation{ pAgent->GetInterface()->Agent_GetInfo().Position };
	const EnemyInfo closestEnemy{ Helpers::GetClosestFromPosition<EnemyInfo>(enemies, agentLocation, [](const EnemyInfo& enemy) { return enemy.Location; }) };

	const float angleBetween{ Helpers::GetAngleBetweenAgentAndEnemy(pAgent, closestEnemy.Location) };
	pBlackboard->SetData(ANGLE_BETWEEN_AGENT_AND_CLOSEST_ENEMY_PARAM, angleBetween);
	pBlackboard->SetData(TARGET_ENEMY_PARAM, closestEnemy);

	const float angularSpeed{ angleBetween > 0.0f ? float(E_PI) : float(-E_PI) };
	pAgent->SetAngularSpeed(angularSpeed);

	const Elite::Vector2 agentToEnemy{ closestEnemy.Location - agentLocation };
	pAgent->SetDestination(agentToEnemy.GetNormalized() * -m_FleeDistance);
}

bool UseWeaponAction::HasTarget(const SurvivalAgentPlugin* pAgent, const Elite::Blackboard* pBlackboard) const
{
	return !pAgent->GetInterface()->GetEnemiesInFOV().empty();
}

bool UseWeaponAction::CanUseItem(const SurvivalAgentPlugin* pAgent, const Elite::Blackboard* pBlackboard) const
{
	float angle;
	return pBlackboard->GetData(ANGLE_BETWEEN_AGENT_AND_CLOSEST_ENEMY_PARAM, angle) && abs(angle) <= m_FacingAngleRadians;
}

void UseWeaponAction::OnAfterUseItem(SurvivalAgentPlugin* pAgent, Elite::Blackboard* pBlackboard) const
{
	EnemyInfo targetEnemy;
	if (!pBlackboard->GetData(TARGET_ENEMY_PARAM, targetEnemy))
		return;

	const std::vector<EnemyInfo> enemiesInFOV{ pAgent->GetInterface()->GetEnemiesInFOV() };

	const auto foundEnemyIterator{ std::ranges::find(enemiesInFOV, targetEnemy) };
	if (foundEnemyIterator == enemiesInFOV.end() || foundEnemyIterator->Health <= 0.0f)
	{
		pAgent->GetMemory().ClearEnemies();
	}

	pBlackboard->RemoveData(TARGET_ENEMY_PARAM);
}
