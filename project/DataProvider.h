#pragma once
#include <unordered_map>

namespace Elite
{
	class Blackboard;
}

typedef std::unordered_map<std::string, bool> WorldState;

// Static class because doesn't have variables/members of its own
class DataProvider
{
public:
	static WorldState GetWorldState(Elite::Blackboard* pBlackboard);
};
