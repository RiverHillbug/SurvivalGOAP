
#pragma once
#include <string>

static const std::string AGENT_PARAM{ "Agent" };

static const std::string ANGLE_BETWEEN_AGENT_AND_CLOSEST_ENEMY_PARAM{ "AngleBetweenAgentAndEnemy" };

static const std::string TARGET_ITEM_PARAM{ "ItemToGrab" };
static const std::string TARGET_HOUSE_PARAM{ "TargetHouse" };
static const std::string WEAPON_SLOTS_PARAM{ "WeaponSlots" };
static const std::string MEDKIT_SLOTS_PARAM{ "MedkitSlots" };
static const std::string FOOD_SLOTS_PARAM{ "FoodSlots" };
static const std::string SEARCHED_HOUSES_PARAM{ "SearchedHouses" };

static const std::string HAS_WEAPON_IN_SIGHT_PARAM{ "HasWeaponInSight" };
static const std::string HAS_MEDKIT_IN_SIGHT_PARAM{ "HasMedkitInSight" };
static const std::string HAS_FOOD_IN_SIGHT_PARAM{ "HasFoodInSight" };
static const std::string HAS_ENEMY_IN_SIGHT_PARAM{ "HasEnemyInSight" };
static const std::string HAS_HOUSE_IN_SIGHT_PARAM{ "HasHouseInSight" };

static const std::string IS_IN_PURGE_ZONE_PARAM{ "IsInPurgeZone" }; // flee

static const std::string HAS_TARGET_WEAPON_PARAM{ "HasTargetWeapon" };
static const std::string HAS_TARGET_FOOD_PARAM{ "HasTargetFood" };
static const std::string HAS_TARGET_MEDKIT_PARAM{ "HasTargetMedkit" };
static const std::string HAS_TARGET_ENEMY_PARAM{ "HasTargetEnemy" };
static const std::string HAS_WEAPON_PARAM{ "HasWeapon" };
static const std::string HAS_MEDKIT_PARAM{ "HasMedkit" };
static const std::string HAS_FOOD_PARAM{ "HasFood" };
static const std::string HAS_INVENTORY_SPACE_PARAM{ "HasInventorySpace" };
static const std::string FILL_INVENTORY_SPACE_PARAM{ "FillInventorySpace" };
static const std::string FOV_STATS_PARAM{ "FOVStats" }; // monitor for world changes

static const std::string HAS_HIGH_HEALTH_PARAM{ "HasHighHealth" }; // use medkit
static const std::string HAS_HIGH_ENERGY_PARAM{ "HasHighEnergy" }; // eat food
static const std::string HAS_HIGH_STAMINA_PARAM{ "HasHighStamina" }; // for running
static const std::string HAS_LOW_STAMINA_PARAM{ "HasLowStamina" };

static const std::string KILL_ENEMY_PARAM{ "KillEnemy" };
static const std::string FLEE_FROM_ENEMY_PARAM{ "FleeFromEnemy" };
static const std::string EXPLORE_PARAM{ "Explore" };
