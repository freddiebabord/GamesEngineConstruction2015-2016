#if !defined (AICOMPONENT)
#define AICOMPONENT

namespace GlassEngine
{
	class Component;

	enum AIState
	{
		Idle,
		Attack,
		Retreat,
		Evade,
		Die,
		Charge
	};

	class AI : public Component
	{
	public:

		AI() : Component(){ id = AIC; };
		AI(SmartPtr<GameObject> parent) : Component(parent){ id = AIC; };
		virtual ~AI(){};

		void Start() override;
		void Update() override;
		void Stop() override;
		
		void State(const AIState state){ currentState = state; };
		const AIState State() const { return currentState; };

		const void ClearTarget() { currentTarget = nullptr; };
		void Target(SmartPtr<GameObject> newTarget){ currentTarget = newTarget; };
		SmartPtr<GameObject> Target() const { return currentTarget; };
		bool HasTarget(){ if (currentTarget == nullptr) return false; else return true; };

	private:
		AIState currentState;
		SmartPtr<GameObject> currentTarget = nullptr;
	};
}

#endif //AICOMPONENT