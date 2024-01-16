#include "stdafx.h"
#include "Memory.h"
#include "SurvivalAgentPlugin.h"
#include "IExamInterface.h"
#include "BlackboardTypes.h"
#include "Helpers.h"
#include <Exam_HelperStructs.h>
#include <algorithm>
#include <xutility>

void Memory::Update(float deltaTime, const SurvivalAgentPlugin* pAgent)
{
	std::erase_if(m_ShortTermMemory,
		[deltaTime, this, pAgent](auto& entityEntry)
		{
			if (IsEntityStaleEnemy(entityEntry.first, pAgent))
			{
				RemoveEntityFromAppropriateVector(entityEntry.first);
				return true;
			}

			entityEntry.second -= deltaTime;

			if (entityEntry.second <= 0.0f)
			{
				RemoveEntityFromAppropriateVector(entityEntry.first);
				return true;
			}
			return false;
		});

	// First, set every new sightings as false, then, if we see any new (that we didn't know of before) of either types, we set that entry to true
	m_NewEntitiesSightings[eEntityType::ITEM] = false;
	m_NewEntitiesSightings[eEntityType::ENEMY] = false;
	m_NewEntitiesSightings[eEntityType::HOUSE] = false;
	m_NewEntitiesSightings[eEntityType::PURGEZONE] = false;

	AddToMemory(pAgent);
}

void Memory::ClearEnemies()
{
	m_Enemies.clear();
	RemoveEnemiesFromShortTermMemory();
}

void Memory::RemoveItem(const ItemInfo& item)
{
	std::erase_if(m_ShortTermMemory,
		[item, this](auto& entityEntry)
		{
			if (entityEntry.first.EntityHash <= item.ItemHash)
			{
				RemoveEntityFromAppropriateVector(entityEntry.first);
				return true;
			}
			return false;
		});
}

std::vector<ItemInfo>& Memory::GetAppropriateItemsVector(const eItemType itemType)
{
	switch (itemType)
	{
	case eItemType::PISTOL:
	case eItemType::SHOTGUN:
		return m_Weapons;

	case eItemType::FOOD:
		return m_Foods;

	case eItemType::MEDKIT:
		return m_Medkits;

	case eItemType::GARBAGE:
		return m_Garbage;
	}

	throw new std::invalid_argument("Trying to get the vector of an unsupported item type");
}

const std::vector<ItemInfo>& Memory::GetAppropriateItemsVector(const eItemType itemType) const
{
	switch (itemType)
	{
	case eItemType::PISTOL:
	case eItemType::SHOTGUN:
		return m_Weapons;

	case eItemType::FOOD:
		return m_Foods;

	case eItemType::MEDKIT:
		return m_Medkits;

	case eItemType::GARBAGE:
		return m_Garbage;
	}

	throw new std::invalid_argument("Trying to get the vector of an unsupported item type");
}

void Memory::AddToMemory(const SurvivalAgentPlugin* pAgent)
{
	for (const auto& item : pAgent->GetInterface()->GetItemsInFOV())
	{
		const EntityInfo info{ eEntityType::ITEM, item.Type, item.Location, item.ItemHash };
		m_ShortTermMemory[info] = m_ShortTermItemMemoryDuration;

		std::vector<ItemInfo>& appropriateVector{ GetAppropriateItemsVector(item.Type) };
		
		if (std::ranges::find(appropriateVector, item) == appropriateVector.end())
		{
			appropriateVector.push_back(item);

			m_NewEntitiesSightings[eEntityType::ITEM] = true;
		}
	}

	bool clearedEnemiesMemory{ false };
	for (const auto& enemy : pAgent->GetInterface()->GetEnemiesInFOV())
	{
		const EntityInfo info{ eEntityType::ENEMY, eItemType::RANDOM_DROP, enemy.Location, enemy.EnemyHash };

		auto iterator{ std::ranges::find(m_Enemies, enemy) };
		if (iterator == m_Enemies.end())
		{
			if (!clearedEnemiesMemory)
			{
				ClearEnemies();
				clearedEnemiesMemory = true;
			}

			m_Enemies.push_back(enemy);

			m_NewEntitiesSightings[eEntityType::ENEMY] = true;
		}

		m_ShortTermMemory[info] = m_ShortTermEnemyMemoryDuration;
	}

	for (const auto& house : pAgent->GetInterface()->GetHousesInFOV())
	{
		if (m_SearchedHouses.count(house) == 0 && std::ranges::find(m_UnsearchedHouses, house) == m_UnsearchedHouses.end())
		{
			m_UnsearchedHouses.push_back(house);

			m_NewEntitiesSightings[eEntityType::HOUSE] = true;
		}
	}

	for (const auto& purgeZone : pAgent->GetInterface()->GetPurgeZonesInFOV())
	{
		const EntityInfo info{ eEntityType::PURGEZONE, eItemType::RANDOM_DROP, purgeZone.Center, purgeZone.ZoneHash };
		m_ShortTermMemory[info] = m_ShortTermPurgeZoneMemoryDuration;

		auto iterator{ std::ranges::find(m_PurgeZones, purgeZone) };
		if (iterator == m_PurgeZones.end())
		{
			m_PurgeZones.push_back(purgeZone);

			m_NewEntitiesSightings[eEntityType::PURGEZONE] = true;
		}
	}
}

bool Memory::IsEntityStaleEnemy(const EntityInfo& entity, const SurvivalAgentPlugin* pAgent)
{
	if (entity.Type != eEntityType::ENEMY)
		return false;

	// If we have a recorded enemy location inside current FOV in memory but the FOV is empty, the enemy info is outdated

	const float agentToEnemyDistanceSqrd{ pAgent->GetInterface()->Agent_GetInfo().Position.DistanceSquared(entity.Location) };
	const float agentRange{ pAgent->GetInterface()->Agent_GetInfo().FOV_Range };

	if (agentToEnemyDistanceSqrd > (agentRange * agentRange))
		return false;

	if (abs(Helpers::GetAngleBetweenAgentAndEnemy(pAgent, entity.Location)) <= (pAgent->GetInterface()->Agent_GetInfo().FOV_Angle * 0.5f))
	{
		return pAgent->GetInterface()->FOV_GetStats().NumEnemies == 0;
	}

	return false;
}

void Memory::RemoveEntityFromAppropriateVector(const EntityInfo& entity)
{
	switch (entity.Type)
	{
	case eEntityType::ENEMY:
		Helpers::RemoveEntityFromVector<EnemyInfo>(entity.EntityHash, m_Enemies, [](const EnemyInfo& enemy) { return enemy.EnemyHash; });
		break;

	case eEntityType::ITEM:
		Helpers::RemoveEntityFromVector<ItemInfo>(entity.EntityHash, GetAppropriateItemsVector(entity.ItemType), [](const ItemInfo& item) { return item.ItemHash; });
		break;

	case eEntityType::PURGEZONE:
		Helpers::RemoveEntityFromVector<PurgeZoneInfo>(entity.EntityHash, m_PurgeZones, [](const PurgeZoneInfo& purgeZone) { return purgeZone.ZoneHash; });
		break;
	}
}

void Memory::RemoveEnemiesFromShortTermMemory()
{
	std::erase_if(m_ShortTermMemory,
		[this](const auto& entityEntry)
		{
			return entityEntry.first.Type == eEntityType::ENEMY;
		});
}

void Memory::MarkHouseAsSearched(const HouseInfo& house)
{
	std::erase(m_UnsearchedHouses, house);
	m_SearchedHouses.insert(house);
}

bool Memory::IsHouseSearched(const HouseInfo& house) const
{
	return m_SearchedHouses.count(house) != 0;
}
