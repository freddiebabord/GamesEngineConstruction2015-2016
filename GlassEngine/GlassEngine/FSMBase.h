#if !defined (AIFSMBASE)
#define AIFSMBASE

#include "Component.h"
#include "FSMState.h"

namespace GlassEngine{


	template <typename T>
	class FSMBase : public Component
	{
	public:
		FSMBase<T>() : Component(){};
		virtual ~FSMBase<T>(){} = 0;

		virtual void Start() override
		{
			CurrentState = nullptr;
			PreviousState = nullptr;
			GloablState = nullptr;
		}

		virtual void Update() override
		{
			if (GloablState)
				GloablState.Update(Owner);
			if (CurrentState)
				CurrentState.Update(Owner);
		}

		virtual void Configure(T Owner_, State<T> initialState)
		{
			Owner = Owner_;
			ChangeState(initialState);
		}

		void ChangeState(State<T> newState)
		{
			if (CurrentState)
				CurrentState.Exit();
			PreviousState = CurrentState;
			CurrentState = newState;
			CurrentState.Enter();
		}

		void RevertToPrevious()
		{
			if (PreviousState)
				ChangeState(PreviousState);
		}

	private:
		T Owner;
		State<T> CurrentState;
		State<T> PreviousState;
		State<T> GloablState;
	};

}
#endif //AIFSMBASE