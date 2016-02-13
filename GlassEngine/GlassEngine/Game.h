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
		
		SmartPtr<Level> loadLevel(char* name);
		SmartPtr<Level> CurrentLevel();
		
		void SpawnBullet(Vec3d position, Vec2d force);
		void SpawnExplosion(Vec3d position);
		
		void Destroy(SmartPtr<GameObject> gameobject);

	protected:
		GameManager(){};

	private:
		SmartPtr<GameObject> CreateNewObject(pugi::xml_node node, int id, SmartPtr<Level> level);
		SmartPtr<Level> loadLevel();
		void AddPrefab(SmartPtr<GameObject> prefab_, std::string key_){ prefabs[key_] = prefab_; };
		void CreateSprite(pugi::xml_node node);
		void CreateSpriteSheet(pugi::xml_node node);
		void CreateBulletPool(int poolSize, int spriteID, pugi::xml_node node, SmartPtr<Level> currentLevel);
		void CreateExplosionPool(int poolSize, int spriteSheetID, SmartPtr<Level> currentLevel, pugi::xml_node node);
	private:
		std::vector<SmartPtr<Level>> levels;
		std::map<std::string, SmartPtr<GameObject>> prefabs;
		static GameManager *instance;
		char* levelFile = "";
		bool levelLoaded = false;
		int currentLevel = 0;
		std::vector<SmartPtr<GameObject>> bullets;
		std::vector<SmartPtr<GameObject>> explosions;
	};

}

#endif //GAME_MANAGER