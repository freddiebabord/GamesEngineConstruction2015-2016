#if !defined (GLASS_ENGINE_ENGINE)
#define GLASS_ENGINE_ENGINE

#include "Manager.h"

namespace GlassEngine{

	class Engine
	{
	public:
		static Engine &Instance();

		void Start(bool debugMode = false, bool fullscreen = true, int width = 800, int height = 500);
		void Update();
		void Stop();

	private:
		Engine(){};

	private:
		static Engine *instance;
		DWORD fixedUpdateTime;
	};


#define GEngine Engine::Instance()

}

#endif //GLASS_ENGINE_ENGINE

