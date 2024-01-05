#include "stdafx.h"
#include "GrabMedkitAction.h"
#include "BlackboardTypes.h"

GrabMedkitAction::GrabMedkitAction()
	: GrabItemAction()
{
	AddPrecondition(HAS_MEDKIT_PARAM, false);
	AddEffect(HAS_MEDKIT_PARAM, true);
}
