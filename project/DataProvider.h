#pragma once
#include <unordered_map>

namespace Elite
{
	class Blackboard;
}

typedef std::unordered_map<std::string, bool> WorldState;
typedef std::unordered_map<std::string, float /*priority*/> Goals;

// Static class because doesn't have variables/members of its own
class DataProvider
{
public:
	static WorldState GetWorldState(Elite::Blackboard* pBlackboard);

private:
	static void CheckHouses(const class SurvivalAgentPlugin* pAgent, Elite::Blackboard* pBlackboard, OUT WorldState& worldState);
	static void CheckItems(const class SurvivalAgentPlugin* pAgent, Elite::Blackboard* pBlackboard, OUT WorldState& worldState);
	static void CheckStats(const class SurvivalAgentPlugin* pAgent, Elite::Blackboard* pBlackboard, OUT WorldState& worldState);
	static void CheckInventory(const class SurvivalAgentPlugin* pAgent, Elite::Blackboard* pBlackboard, OUT WorldState& worldState);

	static void EnterData(const std::string& name, const bool value, Elite::Blackboard* pBlackboard, OUT WorldState& worldState);
};
