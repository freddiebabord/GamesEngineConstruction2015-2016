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

		virtual void OnStart(){};
		virtual void Update(){};

		void SetID(int newID){ id = newID; };
		const int GetID() { return id; };
		std::vector<SmartPtr<GameObject>> GetGameObjects() { return gameObjects; };
		void AddGameObject(SmartPtr<GameObject> newGameObject){ gameObjects.push_back(newGameObject); };
		char* GetLevelName() { return levelName; };
		void DeleteLevel();
		void SetAudioTrack(std::string audio){ audioTrack = audio; };
		std::string GetAudioTrack() { return audioTrack; };

		void AddAudio(std::string clipName, std::string clipPath){
			int id = 0;
			if (HAPI->LoadSound(clipPath, &id))
			{
				audio[clipName] = id;
			}
		};

		int GetAudioID(std::string clipName){ return audio[clipName]; };

	protected:
		GameManager* game = nullptr;
		std::vector<SmartPtr<GameObject>> gameObjects;
		int id = 0;
		char* levelName;
		std::string audioTrack = "";
		std::map<std::string, int> audio;
	};
}
#endif