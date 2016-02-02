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
		~Level(){ gameObjects.clear(); ui.clear(); };

		void SetID(int newID){ id = newID; };
		const int GetID() { return id; };
		std::vector<std::shared_ptr<GameObject>> GetGameObjects() { return gameObjects; };
		std::vector<std::shared_ptr<GameObject>> GetUIObjects() { return ui; };
		void AddGameObject(std::shared_ptr<GameObject> newGameObject){ gameObjects.push_back(newGameObject); };
		void AddUIObject(std::shared_ptr<GameObject> newUIObject){ ui.push_back(newUIObject); };
		char* GetLevelName() { return levelName; };
		void DeleteLevel();
		void SetAudioTrack(std::string audio){ audioTrack = audio; };
		std::string GetAudioTrack() { return audioTrack; };

	private:
		GameManager* game = nullptr;
		std::vector<std::shared_ptr<GameObject>> gameObjects;
		std::vector<std::shared_ptr<GameObject>> ui;
		int id = 0;
		char* levelName;
		std::string audioTrack = "";
	};
}
#endif