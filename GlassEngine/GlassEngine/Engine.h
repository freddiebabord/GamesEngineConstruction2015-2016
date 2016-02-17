#if !defined (GLASS_ENGINE_ENGINE)
#define GLASS_ENGINE_ENGINE

#include "Manager.h"

namespace GlassEngine{
	
	class GameObject;

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
		DWORD engineUpdateTime;
		std::vector<SmartPtr<GameObject>> gameObjects_;
		bool showMask = false;
	};


#define GEngine Engine::Instance()

}

#endif //GLASS_ENGINE_ENGINE

