//=== General Includes ===
#include "stdafx.h"
#include "EFiniteStateMachine.h"
#include "EliteAI\EliteData\EBlackboard.h"

using namespace Elite;

FiniteStateMachine::FiniteStateMachine(FSMState* startState, Blackboard* pBlackboard)
	: m_pCurrentState(nullptr)
	, m_pBlackboard(pBlackboard)
{
	ChangeState(startState);
}

FiniteStateMachine::~FiniteStateMachine()
{
	SAFE_DELETE(m_pBlackboard);
}

void FiniteStateMachine::Update(float deltaTime)
{
	//Update the current state (if one exists)
	if (m_pCurrentState != nullptr)
	{
		m_pCurrentState->Update(m_pBlackboard, deltaTime);
	}
}

Blackboard* FiniteStateMachine::GetBlackboard() const
{
	return m_pBlackboard;
}

void FiniteStateMachine::ChangeState(FSMState* newState)
{
	m_pCurrentState = newState;
}

