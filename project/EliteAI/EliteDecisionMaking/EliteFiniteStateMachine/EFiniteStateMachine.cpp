//=== General Includes ===
#include "stdafx.h"
#include "EFiniteStateMachine.h"
#include "EliteAI\EliteData\EBlackboard.h"
#include "DataProvider.h"
#include <iostream>

using namespace Elite;

void FSMState::AddTransition(const FSMState* toState, const FSMCondition* condition /*= nullptr*/)
{
	m_Transitions[toState] = condition;
}

FiniteStateMachine::~FiniteStateMachine()
{
	for (auto& state : m_AllStates)
	{
		SAFE_DELETE(state);
	}

	for (auto& condition : m_AllConditions)
	{
		SAFE_DELETE(condition);
	}
}

void FiniteStateMachine::Update(float deltaTime)
{
	// Update the current state (if one exists)
	if (m_pCurrentState == nullptr)
		return;

	DataProvider::GetWorldState(m_pBlackboard);

	m_pCurrentState->Update(m_pBlackboard, deltaTime);

	if (m_pCurrentState->IsDone(m_pBlackboard))
	{
		const auto& transitions{ m_pCurrentState->GetTransitions() };
		ExitCurrentState();

		for (const auto& transition : transitions)
		{
			if (transition.second == nullptr || transition.second->Evaluate(m_pBlackboard))
			{
				EnterState(transition.first);
				return;
			}
		}

		std::cout << "No valid state to transition to!\n";
	}
}

void FiniteStateMachine::EnterState(const FSMState* newState)
{
	m_pCurrentState = newState;

	if (m_pCurrentState != nullptr)
	{
		m_pCurrentState->OnEnter(m_pBlackboard);
	}
}

void FiniteStateMachine::ExitCurrentState()
{
	if (m_pCurrentState != nullptr)
	{
		m_pCurrentState->OnExit(m_pBlackboard);
	}

	m_pCurrentState = nullptr;
}
