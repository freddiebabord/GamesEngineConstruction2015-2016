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
		AI(std::shared_ptr<GameObject> parent) : Component(parent){ id = AIC; };
		virtual ~AI(){};

		void Start() override;
		void Update() override;
		void Stop() override;
		
		void State(const AIState state){ currentState = state; };
		const AIState State() const { return currentState; };

		const void ClearTarget() { currentTarget = nullptr; };
		void Target(std::shared_ptr<GameObject> newTarget){ currentTarget = newTarget; };
		std::shared_ptr<GameObject> Target() const { return currentTarget; };
		bool HasTarget(){ if (currentTarget == nullptr) return false; else return true; };

	private:
		AIState currentState;
		std::shared_ptr<GameObject> currentTarget = nullptr;
	};
}

#endif //AICOMPONENT