#if !defined (LEVEL)
#define LEVEL

#include "GameObject.h"



namespace GlassEngine{
	class CPlayer;
class GameManager;

	class Level
	{
	public:
		Level(){};
		Level(GameManager* game_, char* levelName_) :game(game_), levelName(levelName_){};
		~Level(){ gameObjects.clear(); };

		void SetID(int newID){ id = newID; };
		const int GetID() { return id; };
		std::vector<std::shared_ptr<GameObject>> GetGameObjects() { return gameObjects; };
		std::vector<std::shared_ptr<CPlayer>> GetPlayers() { return players; };
		void AddGameObject(std::shared_ptr<GameObject> newGameObject){ gameObjects.push_back(newGameObject); };
		void AddPlayer(std::shared_ptr<CPlayer> newGameObject){ players.push_back(newGameObject); };
		char* GetLevelName() { return levelName; };

	private:
		GameManager* game = nullptr;
		std::vector<std::shared_ptr<GameObject>> gameObjects;
		std::vector<std::shared_ptr<CPlayer>> players;
		int id = 0;
		char* levelName;
	};
}
#endif