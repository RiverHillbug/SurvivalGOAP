/*=============================================================================*/
// Copyright 2020-2021 Elite Engine
/*=============================================================================*/
// StatesAndTransitions.h: Implementation of the state/transition classes
/*=============================================================================*/

#pragma once
#include "EliteAI/EliteData/EBlackboard.h"
#include "EliteAI/EliteDecisionMaking/EliteFiniteStateMachine/EFiniteStateMachine.h"

using namespace Elite;

class PlanningState : public FSMState
{
public:
	PlanningState() {};
	~PlanningState() {};
	void Update(Blackboard* pBlackboard, float deltaTime) override;
};

class MoveToState : public FSMState
{
public:
	MoveToState() {};
	~MoveToState() {};
	void OnEnter(Blackboard* pBlackboard) override;
	void Update(Blackboard* pBlackboard, float deltaTime) override;
};

class PerformActionState : public FSMState
{
public:
	PerformActionState() {};
	~PerformActionState() {};
	void Update(Blackboard* pBlackboard, float deltaTime) override;
};

class NeedsRange : public FSMCondition
{
public:
	NeedsRange() = default;
	~NeedsRange() = default;
	bool Evaluate(const Blackboard* pBlackboard) const override;
};
