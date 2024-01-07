/*=============================================================================*/
// Copyright 2020-2021 Elite Engine
/*=============================================================================*/
// StatesAndTransitions.h: Implementation of the state/transition classes
/*=============================================================================*/

#pragma once
#include "EliteAI/EliteDecisionMaking/EliteFiniteStateMachine/EFiniteStateMachine.h"

namespace Elite
{
	class Blackboard;
}

class PlanningState : public Elite::FSMState
{
public:
	PlanningState() {};
	~PlanningState() {};

	void Update(Elite::Blackboard* pBlackboard, float deltaTime) const override;
	bool IsDone(const Elite::Blackboard* pBlackboard) const override;
};

class MoveToState : public Elite::FSMState
{
public:
	MoveToState() {};
	~MoveToState() {};

	void Update(Elite::Blackboard* pBlackboard, float deltaTime) const override;
	bool IsDone(const Elite::Blackboard* pBlackboard) const override;
};

class PerformActionState : public Elite::FSMState
{
public:
	PerformActionState() {};
	~PerformActionState() {};

	void Update(Elite::Blackboard* pBlackboard, float deltaTime) const override;
	void OnExit(Elite::Blackboard* pBlackboard) const override;

	bool IsDone(const Elite::Blackboard* pBlackboard) const override;
};

class NeedsRangeCondition : public Elite::FSMCondition
{
public:
	NeedsRangeCondition() = default;
	~NeedsRangeCondition() = default;

	bool Evaluate(const Elite::Blackboard* pBlackboard) const override;
};

class HasPlanCondition : public Elite::FSMCondition
{
public:
	HasPlanCondition() = default;
	~HasPlanCondition() = default;

	bool Evaluate(const Elite::Blackboard* pBlackboard) const override;
};
