#if !defined (GLASS_ENGINE_ENGINE)
#define GLASS_ENGINE_ENGINE

#include "Manager.h"
#include "RenderManager.h"
#include "InputManager.h"
#include "PhysicsManager.h"
#include "Time.h"
#include "Game.h"


namespace GlassEngine{

	struct Animation;

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
		GameManager* gameManager;
	};


#define GEngine Engine::Instance()

}

#endif //GLASS_ENGINE_ENGINE

