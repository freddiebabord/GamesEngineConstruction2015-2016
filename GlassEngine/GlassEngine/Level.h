#if !defined (LEVEL)
#define LEVEL

#include "GameObject.h"



namespace GlassEngine{
	
	class GameManager;

	class Level
	{
	public:
		Level(){};
		Level(GameManager* game_, char* levelName_) :game(game_), levelName(levelName_){};
		~Level(){ gameObjects.clear(); };

		void SetID(int newID){ id = newID; };
		const int GetID() { return id; };
		std::vector<SmartPtr<GameObject>> GetGameObjects() { return gameObjects; };
		void AddGameObject(SmartPtr<GameObject> newGameObject){ gameObjects.push_back(newGameObject); };
		char* GetLevelName() { return levelName; };
		void DeleteLevel();
		void SetAudioTrack(std::string audio){ audioTrack = audio; };
		std::string GetAudioTrack() { return audioTrack; };

	private:
		GameManager* game = nullptr;
		std::vector<SmartPtr<GameObject>> gameObjects;
		int id = 0;
		char* levelName;
		std::string audioTrack = "";
	};
}
#endif