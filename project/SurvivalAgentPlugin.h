#pragma once
#include "IExamPlugin.h"
#include "Exam_HelperStructs.h"
#include "DataProvider.h"
#include <set>
#include <unordered_map>

class SurvivalAgentPlugin : public IExamPlugin
{
public:
	SurvivalAgentPlugin();
	virtual ~SurvivalAgentPlugin() {};

	void Initialize(class IBaseInterface* pInterface, PluginInfo& info) override;
	void DllInit() override;
	void DllShutdown() override;

	void InitGameDebugParams(GameDebugParams& params) override;
	void Update_Debug(float dt) override;

	SteeringPlugin_Output UpdateSteering(float dt) override;
	void Render(float dt) const override;

	inline class IExamInterface* GetInterface() const { return m_pInterface; }
	UINT SelectFirstAvailableInventorySlot();
	inline UINT GetSelectedInventorySlot() const { return m_InventorySlot; }

	inline const std::set<const class GOAPAction*>& GetAvailableActions() const { return m_AvailableActions; }
	inline const WorldState& GetGoals() const { return m_Goals; }

private:
	//Interface, used to request data from/perform actions with the AI Framework
	class IExamInterface* m_pInterface{ nullptr };

	class GOAPPlanner* m_pGOAPPlanner;
	WorldState m_Goals{};

	Elite::Vector2 m_Target{};
	bool m_CanRun{ false }; //Demo purpose
	bool m_GrabItem{ false }; //Demo purpose
	bool m_UseItem{ false }; //Demo purpose
	bool m_RemoveItem{ false }; //Demo purpose
	bool m_DestroyItemsInFOV{ false };
	float m_AngSpeed{ 0.0f }; //Demo purpose

	UINT m_InventorySlot{ 0 };

	class FiniteStateMachine* m_pFSM{ nullptr };
	Elite::Blackboard* m_pBlackboard;
	std::set<const class GOAPAction*> m_AvailableActions{};
	std::queue<const class GOAPAction*> m_CurrentPlan{};
};

//ENTRY
//This is the first function that is called by the host program
//The plugin returned by this function is also the plugin used by the host program
extern "C"
{
	__declspec (dllexport) IPluginBase* Register();
}
