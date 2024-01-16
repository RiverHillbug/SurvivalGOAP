#pragma once
#include "FindItemAction.h"

class FindWeaponAction : public FindItemAction
{
public:
	FindWeaponAction();
	~FindWeaponAction() = default;

	eItemType GetItemType() const override { return eItemType::PISTOL; }
};
