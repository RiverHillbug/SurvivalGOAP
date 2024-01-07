#pragma once
#include "FindItemAction.h"

class FindWeaponAction : public FindItemAction
{
public:
	FindWeaponAction();
	~FindWeaponAction() = default;

	bool IsCorrectItemType(const struct ItemInfo& item) const override;
};
