/*=============================================================================*/
// Copyright 2020-2021 Elite Engine
// Authors: Andries Geens
// http://www.gameaipro.com/GameAIPro3/GameAIPro3_Chapter12_A_Reusable_Light-Weight_Finite-State_Machine.pdf
/*=============================================================================*/
// EStateMachine.h: Other implementation of a FSM
/*=============================================================================*/

#pragma once

namespace Elite
{
	class FSMCondition
	{
	public:
		FSMCondition() = default;
		virtual ~FSMCondition() = default;
		virtual bool Evaluate(Blackboard* pBlackboard) const = 0;
	};

	class FSMState
	{
	public:
		FSMState(){}
		virtual ~FSMState() = default;
		void AddTransition(FSMState* toState, const FSMCondition* condition);
		inline std::unordered_map<FSMState*, const FSMCondition*> GetTransitions() const { return m_Transitions; }

		virtual void OnEnter(class Blackboard* pBlackboard);
		virtual void Update(class Blackboard* pBlackboard, float deltaTime) {}
		virtual void OnExit(class Blackboard* pBlackboard) {}

		virtual bool CanEnter() const { return true; }
		inline bool IsDone() const { return m_IsDone; }

	protected:
		bool m_IsDone{ false };

	private:
		std::unordered_map<FSMState*, const FSMCondition*> m_Transitions;
	};

	class FiniteStateMachine final
	{
	public:
		FiniteStateMachine(FSMState* startState, class Blackboard* pBlackboard);
		~FiniteStateMachine();
		
		void Update(float deltaTime);
		inline class Blackboard* GetBlackboard() const { return m_pBlackboard; }
		void EnterState(FSMState* newState);

	private:
		FSMState* m_pCurrentState;
		class Blackboard* m_pBlackboard = nullptr; // takes ownership of the blackboard
	};
}
