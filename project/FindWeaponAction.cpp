#include "stdafx.h"
#include "FindWeaponAction.h"
#include "BlackboardTypes.h"
#include "IExamInterface.h"
#include "SurvivalAgentPlugin.h"
#include "Exam_HelperStructs.h"
#include "EliteAI\EliteData\EBlackboard.h"
#include "EliteMath\EVector2.h"

FindWeaponAction::FindWeaponAction()
{
	AddPrecondition(HAS_WEAPON_IN_SIGHT_PARAM, true);
	AddPrecondition(HAS_TARGET_ITEM_PARAM, false);
	AddPrecondition(HAS_WEAPON_PARAM, false);
	AddPrecondition(HAS_INVENTORY_SPACE_PARAM, true);
	AddEffect(HAS_TARGET_ITEM_PARAM, true);
}

void FindWeaponAction::Reset()
{
	m_IsDone = false;
}

bool FindWeaponAction::Perform(Elite::Blackboard* pBlackboard)
{
	SurvivalAgentPlugin* pAgent{ nullptr };
	if (pBlackboard->GetData(AGENT_PARAM, pAgent))
	{
		std::vector<ItemInfo> weaponsInFOV;
		std::vector<ItemInfo> items;
		//pBlackboard->GetData(ITEMS_IN_FOV_PARAM, items);
		// Get them through the agent

		for (const auto& item : items)
		{
			if (item.Type == eItemType::PISTOL || item.Type == eItemType::SHOTGUN)
			{
				weaponsInFOV.push_back(item);
			}
		}

		ItemInfo closestWeapon;
		const Elite::Vector2 agentLocation{ pAgent->GetInterface()->Agent_GetInfo().Position };
		float closestDistance{ (weaponsInFOV[0].Location - agentLocation).MagnitudeSquared() };

		for (int i{ 0 }; i < int(weaponsInFOV.size()); ++i)
		{
			float distance{ (weaponsInFOV[i].Location - -agentLocation).MagnitudeSquared() };

			if ( distance <= closestDistance)
			{
				closestDistance = distance;
				closestWeapon = weaponsInFOV[i];
			}
		}

		pBlackboard->SetData(TARGET_ITEM_PARAM, closestWeapon);

		m_IsDone = true;
		return true;
	}

	m_IsDone = true;
	return false;
}
