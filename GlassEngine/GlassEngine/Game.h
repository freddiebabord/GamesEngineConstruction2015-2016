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
		void FixedUpdate() override;
		void Update() override;
		void Stop() override;
		
		void DeleteLevels();
		
		std::shared_ptr<Level> loadLevel(char* name);
		std::shared_ptr<Level> CurrentLevel();

		std::shared_ptr<GameObject> Instantiate(std::string gameObjectName);
		std::shared_ptr<GameObject> Instantiate(std::string gameObjectName, Vec3d pos);

	protected:
		GameManager(){};

	private:
		std::shared_ptr<GameObject> CreateNewObject(pugi::xml_node node, int id);
		std::shared_ptr<Level> loadLevel();
		void AddPrefab(std::shared_ptr<GameObject> prefab_, std::string key_){ prefabs[key_] = prefab_; };
		void CreateSprite(pugi::xml_node node);
		void CreateSpriteSheet(pugi::xml_node node);

	private:
		std::vector<std::shared_ptr<Level>> levels;
		std::map<std::string, std::shared_ptr<GameObject>> prefabs;
		static GameManager *instance;
		char* levelFile = "";
		bool levelLoaded = false;
		int currentLevel = 0;
	};

}

#endif //GAME_MANAGER