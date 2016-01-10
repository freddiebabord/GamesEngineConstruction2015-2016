#if !defined (GAME_MANAGER)
#define GAME_MANAGER

#include "XML\pugixml.hpp"
#include "Manager.h"
#include "Level.h"
#include "RenderManager.h"
#include "PhysicsManager.h"

namespace GlassEngine{

#define Game GameManager::Instance()

	class CPlayer;

	class GameManager : public Manager
	{
	public:
		static GameManager &Instance();
		~GameManager(){};

		void Start() override;
		void Update() override;
		void Stop() override;
		
		
		std::shared_ptr<Level> loadLevel(char* name);
		std::shared_ptr<Level> CurrentLevel();

	protected:
		GameManager(){};

	private:
		std::shared_ptr<GameObject> CreateNewObject(pugi::xml_node node, int id);
		std::shared_ptr<Level> loadLevel();

	private:
		std::vector<std::shared_ptr<Level>> levels;

		static GameManager *instance;
		char* levelFile = "";
		bool levelLoaded = false;
		int currentLevel = 0;

	};

}

#endif //GAME_MANAGER