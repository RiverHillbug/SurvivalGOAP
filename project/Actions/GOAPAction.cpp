#include "stdafx.h"
#include "GOAPAction.h"
#include "EliteAI\EliteData\EBlackboard.h"
#include "Helpers.h"
#include <iostream>


void GOAPAction::OnExit(Elite::Blackboard* pBlackboard) const
{
	Helpers::ApplyState(m_Effects, pBlackboard);
}

void GOAPAction::AddPrecondition(const std::string& name, const bool value)
{
	AddToWorldState(name, value, m_Preconditions);
}

void GOAPAction::AddEffect(const std::string& name, const bool value)
{
	AddToWorldState(name, value, m_Effects);
}

void GOAPAction::AddPlanOnlyEffect(const std::string& name, const bool value)
{
	AddToWorldState(name, value,  m_PlanOnlyEffects);
}

void GOAPAction::AddToWorldState(const std::string& name, const bool value, OUT WorldState& worldState)
{
	if (worldState.find(name) != worldState.end())
	{
		std::cout << "WARNING: Data '" << name << "' already in map.\n";
	}

	worldState[name] = value;
}
