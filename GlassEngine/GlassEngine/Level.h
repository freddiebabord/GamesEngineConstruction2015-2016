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
		std::vector<SmartPtr<GameObject>> GetGameObjects() { return gameObjects; };
		std::vector<SmartPtr<GameObject>> GetUIObjects() { return ui; };
		void AddGameObject(SmartPtr<GameObject> newGameObject){ gameObjects.push_back(newGameObject); };
		void AddUIObject(SmartPtr<GameObject> newUIObject){ ui.push_back(newUIObject); };
		char* GetLevelName() { return levelName; };
		void DeleteLevel();
		void SetAudioTrack(std::string audio){ audioTrack = audio; };
		std::string GetAudioTrack() { return audioTrack; };

	private:
		GameManager* game = nullptr;
		std::vector<SmartPtr<GameObject>> gameObjects;
		std::vector<SmartPtr<GameObject>> ui;
		int id = 0;
		char* levelName;
		std::string audioTrack = "";
	};
}
#endif