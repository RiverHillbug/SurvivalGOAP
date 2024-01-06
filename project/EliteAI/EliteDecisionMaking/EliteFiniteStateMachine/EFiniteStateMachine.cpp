//=== General Includes ===
#include "stdafx.h"
#include "EFiniteStateMachine.h"
#include "EliteAI\EliteData\EBlackboard.h"

using namespace Elite;

void FSMState::OnEnter(Blackboard* pBlackboard)
{
	m_IsDone = false;
}

void FSMState::AddTransition(FSMState* toState, const FSMCondition* condition)
{
	m_Transitions[toState] = condition;
}

FiniteStateMachine::FiniteStateMachine(FSMState* startState, Blackboard* pBlackboard)
	: m_pCurrentState(nullptr)
	, m_pBlackboard(pBlackboard)
{
	EnterState(startState);
}

FiniteStateMachine::~FiniteStateMachine()
{
	SAFE_DELETE(m_pBlackboard);
}

void FiniteStateMachine::Update(float deltaTime)
{
	//Update the current state (if one exists)
	if (m_pCurrentState != nullptr)
		return;

	m_pCurrentState->Update(m_pBlackboard, deltaTime);

	if (m_pCurrentState->IsDone())
	{
		auto transitions{ m_pCurrentState->GetTransitions() };
		for (const auto& transition : transitions)
		{
			if (transition.second->Evaluate(m_pBlackboard))
			{
				EnterState(transition.first);
				break;
			}
		}

		// Error: No valid state to transition to
	}
}

void FiniteStateMachine::EnterState(FSMState* newState)
{
	if(m_pCurrentState != nullptr)
	{
		m_pCurrentState->OnExit(m_pBlackboard);
	}

	m_pCurrentState = newState;

	if (m_pCurrentState != nullptr)
	{
		m_pCurrentState->OnEnter(m_pBlackboard);
	}
}
