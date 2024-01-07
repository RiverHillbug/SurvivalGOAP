#pragma once
#pragma region MISC
#include <string>

struct SteeringPlugin_Output
{
	Elite::Vector2 LinearVelocity{ 0.0f, 0.0f };
	float AngularVelocity{ 0.0f };
	bool AutoOrient{ true };
	bool RunMode{ false };
};

struct PluginInfo
{
	std::string BotName{ "NoName" };
	std::string Student_Name{ "John Cena" };
	std::string Student_Class{ "2DAE00" };
	std::string LB_Password{ "ABC123" };
};

struct GameDebugParams //Debuggin Purposes only (Ignored during release build)
{
	GameDebugParams(bool spawnEnemies = true, int enemyCount = 20, bool godMode = false, bool autoFollowCam = false)
	{
		SpawnEnemies = spawnEnemies;
		GodMode = godMode;
		AutoFollowCam = autoFollowCam;
		EnemyCount = enemyCount;
	}

	bool SpawnEnemies{ true }; //Spawn enemies?
	int EnemyCount{ 20 }; //Amount of enemies?
	int ItemCount{ 40 }; //Amount of items?
	bool GodMode{ false }; //Use GodMode? (Invincible)
	bool IgnoreEnergy{ false }; //Ignore energy depletion
	bool AutoFollowCam{ false }; //Auto follow the player
	bool RenderUI{ false }; //Render Player UI (Parameters)
	bool AutoGrabClosestItem{ false }; //Auto Grab closest item (Item_Grab)
	std::string LevelFile{ "GameLevel.gppl" }; //Level to load?
	int Seed{ -1 }; //Seed for random generator
	int StartingDifficultyStage{ 0 }; // Overwrites the difficulty stage
	bool InfiniteStamina{ false }; // Agent has infinite stamina
	bool SpawnDebugPistol{ false }; // Spawns pistol with 1000 ammo at start
	bool SpawnDebugShotgun{ false }; // Spawns shotgun with 1000 ammo at start
	bool SpawnPurgeZonesOnMiddleClick{ false }; // Middle mouse clicks spawn purge zones
	bool SpawnZombieOnRightClick{ false }; // Right mouse clicks spawn purge zones
	bool PrintDebugMessages{ true };
	bool ShowDebugItemNames{ true };
};
#pragma endregion

#pragma region ENTITIES
//Enumerations
//************
enum class eEntityType
{
	ITEM,
	ENEMY,
	PURGEZONE,

	//@END
	_LAST = PURGEZONE
};

enum class eItemType
{
	PISTOL,
	SHOTGUN,
	MEDKIT,
	FOOD,
	GARBAGE,
	RANDOM_DROP, //Internal Only
	RANDOM_DROP_WITH_CHANCE, //Internal Only

	//@END
	_LAST = GARBAGE
};

enum class eEnemyType
{
	DEFAULT,
	ZOMBIE_NORMAL,
	ZOMBIE_RUNNER,
	ZOMBIE_HEAVY,
	RANDOM_ENEMY, //Internal Only
	//...

	//@END
	_LAST = ZOMBIE_HEAVY
};


//Structures
//**********
struct StatisticsInfo
{
	int Score{ 0 }; //Current Score
	float Difficulty{ 0.0f }; //Current difficulty (0 > 1 > ... / Easy > Normal > Hard)
	float TimeSurvived{ 0.0f }; //Total time survived
	float KillCountdown{ 0.0f }; //Make sure to make a kill before this timer runs out

	int NumEnemiesKilled{ 0 }; //Amount of enemies killed
	int NumEnemiesHit{ 0 }; //Amount of enemy hits
	int NumItemsPickUp{ 0 }; //Amount of items picked up
	int NumMissedShots{ 0 }; //Shots missed after firing
	int NumChkpntsReached{ 0 }; //Amount of checkpoints reached

};

struct FOVStats
{
	int NumHouses{ 0 };
	int NumEnemies{ 0 };
	int NumItems{ 0 };
	int NumPurgeZones{ 0 };

	bool operator!=(const FOVStats& other)
	{
		return	NumHouses != other.NumHouses ||
				NumEnemies != other.NumEnemies ||
				NumItems != other.NumItems/* ||
				NumPurgeZones != other.NumPurgeZones*/;	// Ignore purge zones for now, since we don't use them yet for planning
	}
};

struct HouseInfo
{
	Elite::Vector2 Center{};
	Elite::Vector2 Size{};

	bool operator!=(const HouseInfo& other) const
	{
		return	Center != other.Center ||
				Size != other.Size;
	}

	bool operator==(const HouseInfo& other) const
	{
		return !(*this != other);
	}
};

namespace std
{
	// Idek, thanks stack overflow

	template <>
	struct hash<HouseInfo>
	{
		auto operator()(const HouseInfo& house) const -> size_t
		{
			std::stringstream hashStr;
			hashStr << house.Center << house.Size;

			return hash<string>()(hashStr.str());
		}
	};
}

struct EnemyInfo
{
	eEnemyType Type{ eEnemyType::DEFAULT };
	Elite::Vector2 Location{};
	Elite::Vector2 LinearVelocity{};

	int EnemyHash{ 0 };
	float Size{ 0.0f };
	float Health{ 0.0f };
};

struct ItemInfo
{
	eItemType Type{ eItemType::PISTOL };
	Elite::Vector2 Location{};

	int ItemHash{ 0 };
	int Value{ 0 };
};

struct PurgeZoneInfo
{
	Elite::Vector2 Center;
	float Radius{ 0.0f };

	int ZoneHash{ 0 };
};

struct EntityInfo
{
	eEntityType Type{ eEntityType::ITEM };
	Elite::Vector2 Location{};

	int EntityHash{ 0 };
};

struct WorldInfo
{
	Elite::Vector2 Center{};
	Elite::Vector2 Dimensions{};
};

struct AgentInfo
{
	float Stamina{ 0.0f };
	float Health{ 0.0f };
	float Energy{ 0.0f };
	bool RunMode{ false };
	bool IsInHouse{ false };
	bool Bitten{ false };		// agent was bitten by a zombie this frame (for internal use)
	bool WasBitten{ false };	// agent was bitten by a zombie recently (0.5 seconds)
	bool Death{ false };

	float FOV_Angle{ 0.0f };
	float FOV_Range{ 0.0f };

	Elite::Vector2 LinearVelocity{};
	float AngularVelocity{ 0.0f };
	float CurrentLinearSpeed{ 0.0f };
	Elite::Vector2 Position{};
	float Orientation{ 0.0f };
	float MaxLinearSpeed{ 0.0f };
	float MaxAngularSpeed{ 0.0f };
	float GrabRange{ 0.0f };
	float AgentSize{ 0.0f };
};
#pragma endregion