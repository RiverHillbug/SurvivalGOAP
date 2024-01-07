#include "stdafx.h"
#include "UseWeaponAction.h"
#include "BlackboardTypes.h"
#include "SurvivalAgentPlugin.h"
#include "EliteAI/EliteData/EBlackboard.h"
#include "Exam_HelperStructs.h"
#include "IExamInterface.h"
#include <vector>
#include <numeric>

UseWeaponAction::UseWeaponAction()
	: UseItemAction()
{
	AddPrecondition(HAS_WEAPON_PARAM, true);
	AddPrecondition(HAS_ENEMY_IN_SIGHT_PARAM, true);

	AddPlanOnlyEffect(KILL_ENEMY_PARAM, true);

	m_NeedsToFaceTarget = true;
}

void UseWeaponAction::FaceTarget(SurvivalAgentPlugin* pAgent, Elite::Blackboard* pBlackboard) const
{
	const std::vector<EnemyInfo> enemies{ pAgent->GetInterface()->GetEnemiesInFOV() };
	if (enemies.empty())
		return;

	const Elite::Vector2 agentLocation{ pAgent->GetInterface()->Agent_GetInfo().Position };

	const EnemyInfo closestEnemy{ *std::ranges::min_element(enemies,
		[agentLocation](const EnemyInfo& left, const EnemyInfo& right)
		{
			const float distanceLeft{ (left.Location - agentLocation).MagnitudeSquared() };
			const float distanceRight{ (right.Location - agentLocation).MagnitudeSquared() };

			return distanceLeft < distanceRight;
		}) };

	pAgent->SetDestination(closestEnemy.Location);
}
