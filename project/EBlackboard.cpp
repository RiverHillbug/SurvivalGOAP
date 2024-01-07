
#pragma once
#include "stdafx.h"
#include "EliteAI/EliteData/EBlackboard.h"

bool Elite::Blackboard::GetBoolData(const std::string& name) const
{
	bool value;
	return GetData(name, value) ? value : false;
}

void Elite::Blackboard::RemoveData(const std::string& name)
{
	m_BlackboardData.erase(name);
}
