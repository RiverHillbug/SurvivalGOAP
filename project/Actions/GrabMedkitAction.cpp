#include "stdafx.h"
#include "GrabMedkitAction.h"
#include "BlackboardTypes.h"

GrabMedkitAction::GrabMedkitAction()
	: GrabItemAction()
{
	AddPrecondition(HAS_TARGET_MEDKIT_PARAM, true);

	AddEffect(HAS_MEDKIT_PARAM, true);
	AddEffect(HAS_TARGET_MEDKIT_PARAM, false);
}
