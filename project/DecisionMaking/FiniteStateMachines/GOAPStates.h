/*=============================================================================*/
// Copyright 2020-2021 Elite Engine
/*=============================================================================*/
// StatesAndTransitions.h: Implementation of the state/transition classes
/*=============================================================================*/
#ifndef ELITE_APPLICATION_FSM_STATES_TRANSITIONS
#define ELITE_APPLICATION_FSM_STATES_TRANSITIONS

#include "EliteAI/EliteData/EBlackboard.h"
#include "EliteAI/EliteDecisionMaking/EliteFiniteStateMachine/EFiniteStateMachine.h"

using namespace Elite;

class IdleState : public FSMState
{
public:
	IdleState() {};
	~IdleState() {};
	void Update(Blackboard* pBlackboard, float deltaTime) override;
};

class MoveToState : public FSMState
{
public:
	MoveToState() {};
	~MoveToState() {};
	void Update(Blackboard* pBlackboard, float deltaTime) override;
};

class PerformActionState : public FSMState
{
public:
	PerformActionState() {};
	~PerformActionState() {};
	void Update(Blackboard* pBlackboard, float deltaTime) override;
};

#endif