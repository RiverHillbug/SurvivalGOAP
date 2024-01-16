#pragma once
#include "FindItemAction.h"

class FindMedkitAction : public FindItemAction
{
public:
	FindMedkitAction();
	~FindMedkitAction() = default;

	eItemType GetItemType() const override { return eItemType::MEDKIT; }
};
