# Zombie Game AI with GOAP
## Introduction

### What is GOAP?
Goal oriented action planning (GOAP) is a powerful AI system that allows agents to make smart decisions in real time without having to maintain a complex finite state machine.
With GOAP, agents make their own action plans to reach a particular goal while taking the current state of the world around them into account.
Giving our agents the ability to come up with their own plans adds variety to their actions, since two agents could come up with two very different plans to reach a similar goal.

![GOAP](https://github.com/Howest-DAE-GD/gpp-researchtopic-milamoonflower/assets/55928191/9789b716-e406-468d-9869-8545efb1fc66)

### How does GOAP work?
GOAP works by giving a set of goals to the agent, and also providing a specific set of actions that the agent can take.
The agent has knowledge of the world around them, such as available resources nearby or the current state of the environment.
With that knowledge, they plans a series of actions that, in the end, satisfies their goal.
For example, our zombie game agent may be low on health, so their goal would be to heal, and the agent may come up with a plan such as "FindMedkit" -> "GrabMedkit" -> "UseMedkit".
The planning algorithm makes sure that the agent always picks the most optimal plan.


## Elements of GOAP
The GOAP system uses these classes:
  - FiniteStateMachine (FSM)
  - FSMState
  - GOAPAction
  - Data Provider
  - GOAPPlanner
  - AI Agent


### FSM and FSMStates
Our agent has a Finite State Machine that has 3 states:
  - Planning: Attempts to formulate a plan based on current world state.
  - MoveTo: Gets the character in range if an action needs the character to be in range of an object, such as "GrabWeapon" or "GrabFood".
  - PerformActions: Executes the current action until completion, then assertain if the next requires a MoveTo, otherwise excecute the next, repeating until completion of the plan.


### GOAPAction
An action is a single step within a plan that allows our agent to do a specific thing.
Actions have preconditions that must be met before the agent can start executing them, and after they're done, they have effects on the agent or the environment.
They also have a cost of execution which is taken into account when building a plan.
For example, a "UseWeapon" action would require the agent to "HaveWeapon" and as an effect, it would "KillEnemy".

```c++
UseWeaponAction::UseWeaponAction()
	: UseItemAction()
{
    AddPrecondition(HAS_WEAPON_PARAM, true);
    AddPrecondition(HAS_ENEMY_IN_SIGHT_PARAM, true);
    AddEffect(KILL_ENEMY_PARAM, true);
    AddEffect(STAY_ALIVE_PARAM, true);
}
```

### Data Provider
The data provider gets the world state information, such as the inventory state, enemies or consumables in sight etc., and transfers this information to the GOAPPlanner.
These are some of the world state parameters I used in my project:

```
static const std::string HAS_WEAPON_IN_SIGHT_PARAM{ "HasWeaponInSight" };
static const std::string HAS_MEDKIT_IN_SIGHT_PARAM{ "HasMedkitInSight" };
static const std::string HAS_FOOD_IN_SIGHT_PARAM{ "HasFoodInSight" };
static const std::string HAS_ENEMY_IN_SIGHT_PARAM{ "HasEnemyInSight" };
static const std::string HAS_HOUSE_IN_SIGHT_PARAM{ "HasHouseInSight" };
```

And the data provider updates their values:

```c++
std::unordered_map<std::string, bool> DataProvider::GetWorldState(Elite::Blackboard* pBlackboard)
{
	std::unordered_map<std::string, bool> worldState;
        .
        .
        .
        worldState.insert({ HAS_WEAPON_IN_SIGHT_PARAM, pBlackboard->GetBoolData(HAS_WEAPON_IN_SIGHT_PARAM) });
        worldState.insert({ HAS_MEDKIT_IN_SIGHT_PARAM, pBlackboard->GetBoolData(HAS_MEDKIT_IN_SIGHT_PARAM) });
        worldState.insert({ HAS_FOOD_IN_SIGHT_PARAM, pBlackboard->GetBoolData(HAS_FOOD_IN_SIGHT_PARAM) });
}
```

### GOAPPlanner
The GOAPPlanner is the part of the code that looks at each available action's preconditions and effects, compares them with the current world state and comes up with a smart set of actions
for our agent to execute to reach their goal.
Here we first look at all of our available actions and check if any of them fulfils our goal through its effects. If an action doesn't fulfil the goal, we then add its effects to the world state and look at all actions doable with that state (keeping track of which actions we came from before this iteration).
If we find an action that does fulfil the goal, we mark it as a successful node, and continue looking at all the other actions until we checked all possible paths.
We now have a list of all the doable plans that do fulfil our goal, of which we select the cheapest (based on the cumulative cost of all actions in that plan) and return this as our agent's plan.
The reason why I chose to use depth first search here is the number of actions in a plan being relatively low, so the performance gain with a better algorithm would be minimal, compared to the complexity of implementing a more advanced pathing algorithm.

Here's a simplified look at the recursive BuildPlan() function in our GOAPPlanner:

```c++
bool GOAPPlanner::BuildGraph(Node* pParent, vector<Node*> succesfulNodes, Blackboard* pBlackboard, WorldState goal, set<GOAPAction*> availableActions)
{
  for (action : availableActions)
  {
    if (AreAllPreconditionsMet(action, pBlackboard))
    {
      WorldState newState{ ApplyState(pParent->m_CumulativeStates, action->GetEffects()) };
      Node* pNewNode{ new Node(pParent, newState, (pParent->m_Cost + action->GetCost()), action) };

      if (AreAllPreconditionsMet(goal, newState))
      {
        // Found a solution!
        succesfulNodes.push_back(pNewNode);
        return true;
      }
      else
      {
        // No solution yet, test for the remaining actions
        std::set<const GOAPAction*> newAvailableActions{ RemoveAction(action, availableActions) };
        if (BuildGraph(pNewNode, succesfulNodes, pBlackboard, action->GetPreconditions(), newAvailableActions))
          return true;
      }
    }
  }

  return false;
}

```

And here is the simplified Plan() function where BuildGraph() is originally called:

```c++
queue<GOAPAction> Plan(Blackboard)
{
    queue<GOAPAction> plan;

    // Get the agent and world states from Blackboard

    // Try building a graph
    if (!BuildGraph(pStartingNode, succesfulNodes, pBlackboard, goal, pAgent->GetAvailableActions()))
    {
        //No plan!
        return plan;
    }

    const Node* pCheapestNode{ GetCheapestNode(succesfulNodes) };

    // After finding the cheapest plan, we get the actions from nodes
    // And we reverse the plan so that it starts from the first action and reaches the goal in the end
    return UnwindPlan(pCheapestNode);
}
```
## Compraison with FSM and BT
Finite State Machines and Behaviour Trees can get really complex when an agent has a lot of different actions available and it may not be easy to add new actions, states, or goals later, as we need to reconsider all possible connections that may include it. GOAP makes it easier to add those since they are all created as independent pieces, for example, a GOAP action doesn't need to know about the existence of other actions, we can just add it to our available actions and the GOAP planner will use it when needed.

## Conclusion

GOAP was a really interesing topic to learn about and apply, and also a big challenge for me. The hardest part was the initial setup of all the actions, preconditions, effects and states, as a lot of ground work needs to be done before being able to test anything, and a lot of the systems are very abstract until you can actually see them in action.
While making it, there were a lot of points that looked like they could be made more performant or more intelligent, making it clear that this can be a very deep and complex system if needed.
For example, we could implement A* to enhance the planning algorithm, we can enhance when/how the agent decides to recalculate a better plan, if ever, we can have a complex system to decide which goals to prioritize depending on the current state of the world, etc.

## References and Sources
- Vedant Chaudhari's article that explains GOAP: https://medium.com/@vedantchaudhari/goal-oriented-action-planning-34035ed40d0b
- Brent Owens' article about GOAP: https://gamedevelopment.tutsplus.com/goal-oriented-action-planning-for-a-smarter-ai--cms-20793t
- Brent Owens' Unity project that helped clarify the setup: https://github.com/sploreg/goap
- Video explaining how to use GOAP: https://www.youtube.com/watch?v=n6vn7d5R_2c
- GDC Talk from 2015 by Chris Conway, Peter Higley and Eric Jacopin on GOAP: https://www.youtube.com/watch?v=gm7K68663rA

