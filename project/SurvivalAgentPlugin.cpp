#include "stdafx.h"
#include "SurvivalAgentPlugin.h"
#include "IExamInterface.h"
#include "BlackboardTypes.h"
#include "EliteAI\EliteData\EBlackboard.h"
#include "EliteAI\EliteDecisionMaking\EliteFiniteStateMachine\EFiniteStateMachine.h"
#include "DecisionMaking\FiniteStateMachines\GOAPStates.h"
#include "Actions\GOAPActions.h"
#include "GOAPPlanner.h"
#include "Helpers.h"

using namespace std;

SurvivalAgentPlugin::SurvivalAgentPlugin()
{
	InitializeGoals();
}

//Called only once, during initialization
void SurvivalAgentPlugin::Initialize(IBaseInterface* pInterface, PluginInfo& info)
{
	//Retrieving the interface
	//This interface gives you access to certain actions the AI_Framework can perform for you
	m_pInterface = static_cast<IExamInterface*>(pInterface);

	//Information for the leaderboards!
	info.BotName = "MinionExam";
	info.Student_Name = "Sabriye Seher Sevik"; //No special characters allowed. Highscores won't work with special characters.
	info.Student_Class = "2DAE09";
	info.LB_Password = "Doridadu";//Don't use a real password! This is only to prevent other students from overwriting your highscore!

	for (UINT i{ 0 }; i < m_pInterface->Inventory_GetCapacity(); ++i)
	{
		m_UsedInventorySlots.push_back(false);
	}

	m_Blackboard.SetData(AGENT_PARAM, this);

	InitializeAvailableActions();
	InitializeFSM();
}

//Called only once
void SurvivalAgentPlugin::DllInit()
{
	
}

//Called only once
void SurvivalAgentPlugin::DllShutdown()
{
	//Called when the plugin gets unloaded
}

//Called only once, during initialization. Only works in DEBUG Mode
void SurvivalAgentPlugin::InitGameDebugParams(GameDebugParams& params)
{
	params.AutoFollowCam = true; //Automatically follow the AI? (Default = true)
	params.RenderUI = true; //Render the IMGUI Panel? (Default = true)
	params.SpawnEnemies = true; //Do you want to spawn enemies? (Default = true)
	params.EnemyCount = 20; //How many enemies? (Default = 20)
	params.GodMode = false; //GodMode > You can't die, can be useful to inspect certain behaviors (Default = false)
	params.LevelFile = "GameLevel.gppl";
	params.AutoGrabClosestItem = true; //A call to Item_Grab(...) returns the closest item that can be grabbed. (EntityInfo argument is ignored)
	params.StartingDifficultyStage = 1;
	params.InfiniteStamina = false;
	params.SpawnDebugPistol = false;
	params.SpawnDebugShotgun = false;
	params.SpawnPurgeZonesOnMiddleClick = true;
	params.PrintDebugMessages = true;
	params.ShowDebugItemNames = true;
	params.Seed = int(time(nullptr)); //-1 = don't set seed. Any other number = fixed seed //TIP: use Seed = int(time(nullptr)) for pure randomness
}

//Only Active in DEBUG Mode
//(=Use only for Debug Purposes)
void SurvivalAgentPlugin::Update_Debug(float deltaTime)
{
	//Demo Event Code

	//In the end your Agent should be able to walk around without external input
	if (m_pInterface->Input_IsMouseButtonUp(Elite::InputMouseButton::eLeft))
	{
		//Update_Debug target based on input
		Elite::MouseData mouseData = m_pInterface->Input_GetMouseData(Elite::InputType::eMouseButton, Elite::InputMouseButton::eLeft);
		const Elite::Vector2 pos = Elite::Vector2(static_cast<float>(mouseData.X), static_cast<float>(mouseData.Y));
		m_Destination = m_pInterface->Debug_ConvertScreenToWorld(pos);
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_Space))
	{
		m_CanRun = true;
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_Left))
	{
		m_AngSpeed -= Elite::ToRadians(10);
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_Right))
	{
		m_AngSpeed += Elite::ToRadians(10);
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_G))
	{
		m_GrabItem = true;
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_U))
	{
		m_UseItem = true;
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_R))
	{
		m_RemoveItem = true;
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_X))
	{
		m_DestroyItemsInFOV = true;
	}
	else if (m_pInterface->Input_IsKeyboardKeyUp(Elite::eScancode_Space))
	{
		m_CanRun = false;
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_Delete))
	{
		m_pInterface->RequestShutdown();
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_KP_Minus))
	{
		if (m_InventorySlot > 0)
			--m_InventorySlot;
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_KP_Plus))
	{
		if (m_InventorySlot < 4)
			++m_InventorySlot;
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_Q))
	{
		ItemInfo info = {};
		m_pInterface->Inventory_GetItem(m_InventorySlot, info);
		std::cout << (int)info.Type << std::endl;
	}
}

//This function calculates the new SteeringOutput, called once per frame
SteeringPlugin_Output SurvivalAgentPlugin::UpdateSteering(float deltaTime)
{
	auto steering = SteeringPlugin_Output();

	m_Memory.Update(deltaTime, this);
	const WorldState currentWorldState{ DataProvider::GetWorldState(&m_Blackboard, m_Memory) };

	if (!m_CurrentPlan.empty() && Helpers::ShouldConsiderNewPlan(currentWorldState, m_PreviousWorldState))
	{
		m_PreviousWorldState = currentWorldState;

		AbortCurrentPlan();
		return steering;
	}

	m_PreviousWorldState = currentWorldState;

	m_FSM.Update(deltaTime);

	//Use the Interface (IAssignmentInterface) to 'interface' with the AI_Framework
	auto agentInfo = m_pInterface->Agent_GetInfo();

	const float stuckDistanceSqrd{ 0.1f };
	if (m_LastPosition.DistanceSquared(agentInfo.Position) > stuckDistanceSqrd)
	{
		m_Blackboard.SetData(IS_NOT_STUCK_PARAM, true);
		m_LastPosition = agentInfo.Position;
		m_CurrentStuckDuration = 0.0f;
	}
	else
	{
		m_CurrentStuckDuration += deltaTime;

		if (m_CurrentStuckDuration >= m_StuckDurationThreshold)
		{
			m_Blackboard.SetData(IS_NOT_STUCK_PARAM, false);
			AbortCurrentPlan();
			m_CurrentStuckDuration = 0.0f;

			return steering;
		}
	}


	//Use the navmesh to calculate the next navmesh point
	//auto nextTargetPos = m_pInterface->NavMesh_GetClosestPathPoint(checkpointLocation);

	//OR, Use the mouse target
	auto nextTargetPos = m_pInterface->NavMesh_GetClosestPathPoint(m_Destination);


// 	if (m_DestroyItemsInFOV)
// 	{
// 		for (auto& item : vItemsInFOV)
// 		{
// 			m_pInterface->DestroyItem(item);
// 		}
// 	}

	//Simple Seek Behaviour (towards Target)
	steering.LinearVelocity = nextTargetPos - agentInfo.Position; //Desired Velocity
	//steering.LinearVelocity = m_Destination - agentInfo.Position; //Desired Velocity
	steering.LinearVelocity.Normalize(); //Normalize Desired Velocity
	steering.LinearVelocity *= agentInfo.MaxLinearSpeed; //Rescale to Max Speed

	if (IsApproximatelyAt(m_Destination))
	{
		steering.LinearVelocity = Elite::ZeroVector2;
	}

	steering.AngularVelocity = m_AngSpeed; //Rotate your character to inspect the world while walking

	steering.AutoOrient = m_AutoOrient; //Setting AutoOrient to true overrides the AngularVelocity
	steering.RunMode = m_CanRun; //If RunMode is True > MaxLinearSpeed is increased for a limited time (until your stamina runs out)

	m_GrabItem = false; //Reset State
	m_UseItem = false;
	m_RemoveItem = false;
	m_DestroyItemsInFOV = false;

	return steering;
}

//This function should only be used for rendering debug elements
void SurvivalAgentPlugin::Render(float dt) const
{
	//This Render function should only contain calls to Interface->Draw_... functions
	m_pInterface->Draw_SolidCircle(m_Destination, 0.7f, { 0, 0 }, { 1, 0, 0 });
}

UINT SurvivalAgentPlugin::SelectFirstAvailableInventorySlot()
{
	const UINT slot{ GetFirstAvailableInventorySpace() };

	if (slot != INVALID_INVENTORY_SLOT)
	{
		m_InventorySlot = slot;
	}
	else
	{
		std::cout << "Inventory full.\n";
	}

	return slot;
}

bool SurvivalAgentPlugin::GrabItem(const ItemInfo& item)
{
	if (m_pInterface->GrabItem(item))
	{
		m_Memory.RemoveItem(item);
		return true;
	}

	return false;
}

bool SurvivalAgentPlugin::DestroyItem(const ItemInfo& item)
{
	if (m_pInterface->DestroyItem(item))
	{
		m_Memory.RemoveItem(item);
		return true;
	}

	return false;
}

bool SurvivalAgentPlugin::Inventory_AddItem(UINT slotId, const ItemInfo& item)
{
	m_UsedInventorySlots[slotId] = true;
	return m_pInterface->Inventory_AddItem(slotId, item);
}

bool SurvivalAgentPlugin::Inventory_RemoveItem(UINT slotId)
{
	m_UsedInventorySlots[slotId] = false;
	return m_pInterface->Inventory_RemoveItem(slotId);
}

bool SurvivalAgentPlugin::HasInventorySpace() const
{
	return GetFirstAvailableInventorySpace() != INVALID_INVENTORY_SLOT;
}

bool SurvivalAgentPlugin::TryPlan(OUT std::queue<const GOAPAction*>& plan)
{
	plan = m_GOAPPlanner.Plan(&m_Blackboard);
	return !plan.empty();
}


void SurvivalAgentPlugin::SetPlan(const std::queue<const class GOAPAction*>& plan)
{
	m_CurrentPlan = plan;
	m_CurrentStuckDuration = 0.0f;
}

bool SurvivalAgentPlugin::IsApproximatelyAt(const Elite::Vector2& position) const
{
	const AgentInfo& agentInfo{ m_pInterface->Agent_GetInfo() };

	const float distanceToleranceSqd{ agentInfo.GrabRange * agentInfo.GrabRange };
	const float distSqrd{ DistanceSquared(position, agentInfo.Position) };
	return distSqrd <= distanceToleranceSqd;
}

void SurvivalAgentPlugin::InitializeFSM()
{
	PlanningState* pPlanningState{ new PlanningState() };
	MoveToState* pMoveToState{ new MoveToState() };
	PerformActionState* pPerformActionState{ new PerformActionState() };
	const NeedsRangeCondition* pNeedsRangeCondition{ new NeedsRangeCondition() };
	const HasPlanCondition* pHasPlanCondition{ new HasPlanCondition() };

	pPlanningState->AddTransition(pMoveToState, pNeedsRangeCondition);
	pPlanningState->AddTransition(pPerformActionState);

	pMoveToState->AddTransition(pPerformActionState);

	pPerformActionState->AddTransition(pMoveToState, pNeedsRangeCondition);
	pPerformActionState->AddTransition(pPerformActionState, pHasPlanCondition);
	pPerformActionState->AddTransition(pPlanningState);

	m_FSM.SetBlackboard(&m_Blackboard);
	m_FSM.AddState(pPlanningState);
	m_FSM.AddState(pMoveToState);
	m_FSM.AddState(pPerformActionState);
	m_FSM.AddCondition(pNeedsRangeCondition);
	m_FSM.AddCondition(pHasPlanCondition);
	m_FSM.EnterState(pPlanningState);
}

void SurvivalAgentPlugin::InitializeGoals()
{
	m_Goals.emplace(IS_NOT_IN_PURGE_ZONE_PARAM, 150.0f);
	m_Goals.emplace(KILL_ENEMY_PARAM, 90.0f);
	m_Goals.emplace(HAS_HIGH_ENERGY_PARAM, 70.0f);
	m_Goals.emplace(HAS_HIGH_HEALTH_PARAM, 60.0f);
	m_Goals.emplace(IS_NOT_STUCK_PARAM, 55.0f);
	m_Goals.emplace(HAS_WEAPON_PARAM, 25.0f);
	m_Goals.emplace(HAS_FOOD_PARAM, 10.0f);
	m_Goals.emplace(HAS_MEDKIT_PARAM, 10.0f);
	m_Goals.emplace(DESTROY_GARBAGE_PARAM, 8.0f);
	m_Goals.emplace(FILL_INVENTORY_SPACE_PARAM, 5.0f);
	m_Goals.emplace(FLEE_FROM_ENEMY_PARAM, 2.0f);
	m_Goals.emplace(EXPLORE_PARAM, 1.0f);
}

void SurvivalAgentPlugin::InitializeAvailableActions()
{
	m_AvailableActions.insert(new FindWeaponAction());
	m_AvailableActions.insert(new FindFoodAction());
	m_AvailableActions.insert(new FindMedkitAction());
	m_AvailableActions.insert(new FindGarbageAction());
	m_AvailableActions.insert(new GrabWeaponAction());
	m_AvailableActions.insert(new GrabFoodAction());
	m_AvailableActions.insert(new GrabMedkitAction());
	m_AvailableActions.insert(new DestroyGarbageAction());
	m_AvailableActions.insert(new UseWeaponAction());
	m_AvailableActions.insert(new UseFoodAction());
	m_AvailableActions.insert(new UseMedkitAction());
	m_AvailableActions.insert(new ExploreAction(m_pInterface->World_GetInfo()));
	m_AvailableActions.insert(new SearchHouseAction(m_pInterface->World_GetInfo()));
	m_AvailableActions.insert(new FleeFromEnemyAction());
	m_AvailableActions.insert(new FleePurgeZoneAction());
}

UINT SurvivalAgentPlugin::GetFirstAvailableInventorySpace() const
{
	for (UINT i{ 0 }; i < UINT(m_UsedInventorySlots.size()); ++i)
	{
		if (!m_UsedInventorySlots[i])
		{
			return i;
		}
	}

	return INVALID_INVENTORY_SLOT;
}

void SurvivalAgentPlugin::AbortCurrentPlan()
{
	m_CurrentPlan = std::queue<const GOAPAction*>();
}

IPluginBase* Register()
{
	return new SurvivalAgentPlugin();
}
