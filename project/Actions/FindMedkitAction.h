#pragma once
#include "FindItemAction.h"

class FindMedkitAction : public FindItemAction
{
public:
	FindMedkitAction();
	~FindMedkitAction() = default;

	bool IsCorrectItemType(const struct ItemInfo& item) const override;
};
