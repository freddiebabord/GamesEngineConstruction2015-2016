#if !defined (ASISTATEMOVE)
#define ASISTATEMOVE

#include "precomp.h"

namespace GlassEngine{

	// TODO: Finish Me

	template <typename T>
	class AIMoveState : public State
	{
	public:
		State<T>(T Owner){};
		~State<T>(){};
		virtual void Enter(T Owner) override;
		virtual void Update(T Owner) override;
		virtual void Exit(T Owner) override;
	};

}

#endif //ASISTATEMOVE