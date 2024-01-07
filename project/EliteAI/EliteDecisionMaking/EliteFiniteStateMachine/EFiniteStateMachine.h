/*=============================================================================*/
// Copyright 2020-2021 Elite Engine
// Authors: Andries Geens
// http://www.gameaipro.com/GameAIPro3/GameAIPro3_Chapter12_A_Reusable_Light-Weight_Finite-State_Machine.pdf
/*=============================================================================*/
// EStateMachine.h: Other implementation of a FSM
/*=============================================================================*/

#pragma once
#include <unordered_map>

namespace Elite
{
	class FSMCondition
	{
	public:
		FSMCondition() = default;
		virtual ~FSMCondition() = default;
		virtual bool Evaluate(const class Blackboard* pBlackboard) const = 0;
	};

	class FSMState
	{
	public:
		FSMState() {}
		virtual ~FSMState() = default;
		void AddTransition(const FSMState* toState, const FSMCondition* condition = nullptr);
		inline std::unordered_map<const FSMState*, const FSMCondition*> GetTransitions() const { return m_Transitions; }

		virtual void OnEnter(class Blackboard* pBlackboard) const {};
		virtual void Update(class Blackboard* pBlackboard, float deltaTime) const {}
		virtual void OnExit(class Blackboard* pBlackboard) const {}

		virtual bool CanEnter() const { return true; }
		virtual bool IsDone(const class Blackboard* pBlackboard) const = 0;

	private:
		std::unordered_map<const FSMState*, const FSMCondition*> m_Transitions;
	};

	class FiniteStateMachine final
	{
	public:
		FiniteStateMachine() = default;
		~FiniteStateMachine();

		inline void SetBlackboard(class Blackboard* pBlackboard) { m_pBlackboard = pBlackboard; }
		inline void AddState(const FSMState* state) { m_AllStates.push_back(state); }
		inline void AddCondition(const FSMCondition* condition) { m_AllConditions.push_back(condition); }

		void Update(float deltaTime);
		inline class Blackboard* GetBlackboard() const { return m_pBlackboard; }
		void EnterState(const FSMState* newState);
		void ExitCurrentState();

	private:
		class Blackboard* m_pBlackboard{ nullptr };
		std::vector<const FSMState*> m_AllStates{};
		std::vector<const FSMCondition*> m_AllConditions{};
		const FSMState* m_pCurrentState{ nullptr };
	};
}
