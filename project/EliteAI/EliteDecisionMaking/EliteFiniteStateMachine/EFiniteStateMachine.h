/*=============================================================================*/
// Copyright 2020-2021 Elite Engine
// Authors: Andries Geens
// http://www.gameaipro.com/GameAIPro3/GameAIPro3_Chapter12_A_Reusable_Light-Weight_Finite-State_Machine.pdf
/*=============================================================================*/
// EStateMachine.h: Other implementation of a FSM
/*=============================================================================*/
//#ifndef ELITE_FINITE_STATE_MACHINE
//#define ELITE_FINITE_STATE_MACHINE
#pragma once
#include <map>
#include <stack>
#include "EliteAI\EliteDecisionMaking\EDecisionMaking.h"

namespace Elite
{
	class FSMState
	{
	public:
		FSMState(){}
		virtual ~FSMState() = default;

		virtual void OnEnter(Blackboard* pBlackboard) {};
		virtual void OnExit(Blackboard* pBlackboard) {};
		virtual void Update(Blackboard* pBlackboard, float deltaTime) {};
	};

	class FSMCondition
	{
	public:
		FSMCondition() = default;
		virtual ~FSMCondition() = default;
		virtual bool Evaluate(Blackboard* pBlackboard) const = 0;
	};

	class FiniteStateMachine final : public Elite::IDecisionMaking
	{
	public:
		FiniteStateMachine(FSMState* startState, Blackboard* pBlackboard);
		virtual ~FiniteStateMachine();
		
		virtual void Update(float deltaTime) override;
		class Blackboard* GetBlackboard() const;
		void ChangeState(FSMState* newState);

	private:
		FSMState* m_pCurrentState;
		Blackboard* m_pBlackboard = nullptr; // takes ownership of the blackboard
	};
}
//#endif