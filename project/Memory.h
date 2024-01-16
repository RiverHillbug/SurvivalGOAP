
#pragma once
#include <unordered_map>
#include <unordered_set>
#include <Exam_HelperStructs.h>

class Memory
{
public:
	Memory() = default;
	~Memory() = default;

	void Update(float deltaTime, const class SurvivalAgentPlugin* pAgent);

	void ClearEnemies();
	void RemoveItem(const ItemInfo& item);
	void MarkHouseAsSearched(const HouseInfo& house);

	bool IsHouseSearched(const HouseInfo& house) const;

	inline const std::unordered_map<EntityInfo, float>& GetShortTermMemory() const { return m_ShortTermMemory; }
	inline const std::vector<EnemyInfo>& GetEnemiesInMemory() const { return m_Enemies; }
	inline const std::vector<ItemInfo>& GetWeaponsInMemory() const { return m_Weapons; }
	inline const std::vector<ItemInfo>& GetFoodsInMemory() const { return m_Foods; }
	inline const std::vector<ItemInfo>& GetMedkitsInMemory() const { return m_Medkits; }
	inline const std::vector<ItemInfo>& GetGarbageInMemory() const { return m_Garbage; }
	inline const std::vector<HouseInfo>& GetUnsearchedHouses() const { return m_UnsearchedHouses; }
	inline const std::vector<PurgeZoneInfo>& GetPurgeZonesInMemory() const { return m_PurgeZones; }

	std::vector<ItemInfo>& GetAppropriateItemsVector(const eItemType itemType);
	const std::vector<ItemInfo>& GetAppropriateItemsVector(const eItemType itemType) const;

	// Did we see any new [entityType] this frame?
	inline bool HasSeenAnyNew(const eEntityType entityType) const { return m_NewEntitiesSightings.at(entityType); }

private:
	std::unordered_map<EntityInfo, float> m_ShortTermMemory{};	// Items and enemies
	std::unordered_map<eEntityType, bool> m_NewEntitiesSightings{};	// Table to know if we saw anything new of a type, this frame

	std::vector<EnemyInfo> m_Enemies{};
	std::vector<ItemInfo> m_Weapons{};
	std::vector<ItemInfo> m_Foods{};
	std::vector<ItemInfo> m_Medkits{};
	std::vector<ItemInfo> m_Garbage{};

	// Houses are treated as long term memory -> never forgetti
	std::unordered_set<HouseInfo> m_SearchedHouses{};
	std::vector<HouseInfo> m_UnsearchedHouses{};

	std::vector<PurgeZoneInfo> m_PurgeZones{};

	const float m_ShortTermEnemyMemoryDuration{ 5.0f };
	const float m_ShortTermPurgeZoneMemoryDuration{ 5.0f };
	const float m_ShortTermItemMemoryDuration{ 60.0f };

	void AddToMemory(const class SurvivalAgentPlugin* pAgent);

	bool IsEntityStaleEnemy(const EntityInfo& entity, const SurvivalAgentPlugin* pAgent);

	void RemoveEntityFromAppropriateVector(const EntityInfo& entity);
	void RemoveEnemiesFromShortTermMemory();
};
