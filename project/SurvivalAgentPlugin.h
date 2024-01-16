#pragma once
#include "IExamPlugin.h"
#include "Exam_HelperStructs.h"
#include "DataProvider.h"
#include "GOAPPlanner.h"
#include "EliteAI/EliteData/EBlackboard.h"
#include "EliteAI/EliteDecisionMaking/EliteFiniteStateMachine/EFiniteStateMachine.h"
#include <set>
#include <unordered_map>
#include "Memory.h"

class SurvivalAgentPlugin : public IExamPlugin
{
public:
	SurvivalAgentPlugin();
	virtual ~SurvivalAgentPlugin() = default;

	void Initialize(class IBaseInterface* pInterface, PluginInfo& info) override;
	void DllInit() override;
	void DllShutdown() override;

	void InitGameDebugParams(GameDebugParams& params) override;
	void Update_Debug(float dt) override;

	SteeringPlugin_Output UpdateSteering(float dt) override;
	void Render(float dt) const override;

	inline class IExamInterface* GetInterface() const { return m_pInterface; }
	bool HasInventorySpace() const;
	UINT SelectFirstAvailableInventorySlot();
	inline UINT GetSelectedInventorySlot() const { return m_InventorySlot; }

	bool GrabItem(const ItemInfo& item);
	bool Inventory_AddItem(UINT slotId, const ItemInfo& item);
	bool Inventory_RemoveItem(UINT slotId);

	inline const std::set<const class GOAPAction*>& GetAvailableActions() const { return m_AvailableActions; }
	inline const Goals& GetGoals() const { return m_Goals; }

	bool TryPlan(OUT std::queue<const class GOAPAction*>& plan);
	void SetPlan(const std::queue<const class GOAPAction*>& plan);
	inline std::queue<const class GOAPAction*>& GetPlan() { return m_CurrentPlan; }
	inline const std::queue<const class GOAPAction*>& GetPlan() const { return m_CurrentPlan; }

	inline Memory& GetMemory() { return m_Memory; }
	inline const Memory& GetMemory() const { return m_Memory; }

	inline void SetDestination(const Elite::Vector2& destination) { m_Destination = destination; }
	inline const Elite::Vector2& GetDestination() const { return m_Destination; }

	inline void SetAngularSpeed(const float speed) { m_AngSpeed = speed; }
	inline void SetAutoOrient(const bool autoOrient) { m_AutoOrient = autoOrient; }
	inline void SetCanRun(const bool canRun) { m_CanRun = canRun; }

	bool IsApproximatelyAt(const Elite::Vector2& position) const;

private:
	void InitializeFSM();
	void InitializeGoals();
	void InitializeAvailableActions();
	UINT GetFirstAvailableInventorySpace() const;
	void AbortCurrentPlan();

	//Interface, used to request data from/perform actions with the AI Framework
	class IExamInterface* m_pInterface{ nullptr };

	GOAPPlanner m_GOAPPlanner{};
	Goals m_Goals{};

	Memory m_Memory{};

	Elite::Vector2 m_Destination{};
	bool m_CanRun{ false }; //Demo purpose
	bool m_GrabItem{ false }; //Demo purpose
	bool m_UseItem{ false }; //Demo purpose
	bool m_RemoveItem{ false }; //Demo purpose
	bool m_DestroyItemsInFOV{ false };
	float m_AngSpeed{ 0.0f }; //Demo purpose
	bool m_AutoOrient{ true };

	UINT m_InventorySlot{ 0 };
	std::vector<bool> m_UsedInventorySlots{};

	Elite::FiniteStateMachine m_FSM{};
	Elite::Blackboard m_Blackboard{};
	WorldState m_PreviousWorldState{};
	std::set<const class GOAPAction*> m_AvailableActions{};
	std::queue<const class GOAPAction*> m_CurrentPlan{};

	Elite::Vector2 m_LastPosition;
	const float m_StuckDurationThreshold{ 3.0f };
	float m_CurrentStuckDuration{ 0.0f };
};

//ENTRY
//This is the first function that is called by the host program
//The plugin returned by this function is also the plugin used by the host program
extern "C"
{
	__declspec (dllexport) IPluginBase* Register();
}
