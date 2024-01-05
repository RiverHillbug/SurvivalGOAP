#include "stdafx.h"
#include "GOAPAction.h"
#include "EliteAI\EliteData\EBlackboard.h"

void GOAPAction::Reset()
{
	m_IsDone = false;
}

void GOAPAction::AddPrecondition(const std::string& name, const bool value)
{
	if (m_Preconditions.find(name) != m_Preconditions.end())
	{
		printf("WARNING: Data '%s' already in preconditions.\n", name.c_str());
	}

	m_Preconditions[name] = value;
}

void GOAPAction::AddEffect(const std::string& name, const bool value)
{
	if (m_Effects.find(name) != m_Effects.end())
	{
		printf("WARNING: Data '%s' already in effects.\n", name.c_str());
	}

	m_Effects[name] = value;
}
