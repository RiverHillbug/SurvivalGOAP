
#pragma once
#include "stdafx.h"
#include "EliteAI/EliteData/EBlackboard.h"

bool Elite::Blackboard::GetBoolData(const std::string& name) const
{
	bool value;
	return GetData(name, value) ? value : false;
}
