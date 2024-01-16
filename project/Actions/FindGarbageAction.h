#pragma once
#include "FindItemAction.h"

class FindGarbageAction : public FindItemAction
{
public:
	FindGarbageAction();
	~FindGarbageAction() = default;

	eItemType GetItemType() const override { return eItemType::GARBAGE; }
};
