#if !defined (AISTATE)
#define AISTATE

#include "precomp.h"

namespace GlassEngine{

	template <typename T>
	class State
	{
	public:
		State<T>(T Owner){};
		~State<T>(){};
		virtual void Enter(T Owner) = 0;
		virtual void Update(T Owner) = 0;
		virtual void Exit(T Owner) = 0;
	};

}

#endif //AISTATE