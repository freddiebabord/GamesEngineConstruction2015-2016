#if !defined (GAME_MANAGER)
#define GAME_MANAGER

#include "XML\pugixml.hpp"
#include "Manager.h"
#include "Level.h"
#include "RenderManager.h"
#include "PhysicsManager.h"
#include "Animator.h"
#include "UIManager.h"
#include "Text.h"
#include "Button.h"
#include "Font.h"
#include "UISprite.h"

namespace GlassEngine{

#define Game GameManager::Instance()

	enum Difficulty
	{
		Easy = 1,
		Normal = 2,
		Hard = 4,
		Insane = 8
	};

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


		void LoadNewLevel(char* levelFileName);
		SmartPtr<Level> CurrentLevel();
		void ReloadGame();

		SmartPtr<GameObject> SpawnBullet(Vec3d position, Vec2d force, Tag tag_, int bulletID = 1);
		void SpawnExplosion(Vec3d position, int explosionSpriteID = 2);

		void Destroy(SmartPtr<GameObject> gameobject);
		void AddLevel(SmartPtr<Level> newLevel);

		void IncreaseKillCount(int amount = 1){ enemiesKilled += amount; };
		void IncreaseScore(int scoreToAdd = 1){ currentGroupScore += (scoreToAdd * difficulty); };

		int GetScore() const { return currentGroupScore; };
		int GetKillCount() const { return enemiesKilled; };

		void SetDifficulty(const Difficulty newDifficulty){ difficulty = newDifficulty; };
		Difficulty GetDifficulty() const { return difficulty; };

		void GameOver(bool hasWon, int score);

		bool IsGameOver() const { return gameShouldEnd; };
		void SetGameOver(const bool overState) { gameShouldEnd = overState; };

		void Reset();
	
	protected:
		GameManager(){};

	private:
		SmartPtr<Level> loadLevel(char* levelFileName);
		void AddPrefab(SmartPtr<GameObject> prefab_, std::string key_){ prefabs[key_] = prefab_; };
		void CreateSprite(pugi::xml_node node);
		void CreateUISprite(pugi::xml_node node);
		void CreateSpriteSheet(pugi::xml_node node);
		void CreateBulletPool(int poolSize, int spriteID, pugi::xml_node node, SmartPtr<Level> currentLevel);
		void CreateExplosionPool(int poolSize, int spriteSheetID, SmartPtr<Level> currentLevel, pugi::xml_node node);
		void CreateNewUIObject(pugi::xml_node node, int id);
	
	private:
		std::vector<SmartPtr<Level>> levels;
		std::map<std::string, SmartPtr<GameObject>> prefabs;
		static GameManager *instance;
		char* levelFile = "";
		bool levelLoaded = false;
		int currentLevel = 0;
		std::vector<SmartPtr<GameObject>> bullets;
		std::vector<SmartPtr<GameObject>> explosions;
		
		int enemiesKilled = 0;
		int currentGroupScore = 0;
		Difficulty difficulty = Difficulty::Normal;

		bool gameShouldEnd = false;

	private:
		template <class ClassType>
		SmartPtr<ClassType> CreateNewObject(pugi::xml_node node, int id, SmartPtr<Level> level)
		{
			SmartPtr<ClassType> newObject = MakeSmartPtr<ClassType>(id);
			SmartPtr<Transform> transform = MakeSmartPtr<Transform>(newObject);
			SmartPtr<Rigidbody> rigidBody = MakeSmartPtr<Rigidbody>(newObject);
			SmartPtr<Collider> collider = MakeSmartPtr<Collider>(newObject);
			SmartPtr<SpriteCollider> spriteCollider = MakeSmartPtr<SpriteCollider>(newObject);
			SmartPtr<Animation> animation = MakeSmartPtr<Animation>(newObject);
			SmartPtr<PointGravity> pointGravity = MakeSmartPtr<PointGravity>(newObject);
			SmartPtr<Health> health = MakeSmartPtr<Health>(newObject);

			newObject->SetName(node.attribute("name").as_string());
			transform->SetPosition(Vec3d(node.child("Transform").attribute("positionX").as_double(), node.child("Transform").attribute("positionY").as_double(), node.child("Transform").attribute("positionZ").as_double()));

			newObject->AddComponent(transform);

			Vec2d offset(0.0);

			if (node.child("Sprite"))
			{
				newObject->SpriteRef(node.child("Sprite").attribute("id").as_int());
				newObject->ColliderRef(node.child("Sprite").attribute("id").as_int());
			}
			if (node.child("SpriteSheet"))
			{
				newObject->ColliderRef(node.child("SpriteSheet").attribute("id").as_int());
				newObject->SpriteSheetRef(node.child("SpriteSheet").attribute("id").as_int());
			}

			if (node.child("Animator"))
			{
				animation->SetAnimSpeed(node.child("Animator").attribute("animationSpeed").as_int());
				for (pugi::xml_node anim : node.child("Animator").children("Animation"))
				{
					animation->AddAnimation(AnimationClip(anim.attribute("name").as_string(), Vec2i(anim.attribute("start").as_int(), anim.attribute("end").as_int())));
				}
				animation->SetProperties();
				newObject->AddComponent(animation);
				Animator.AddAnimation(animation);
			}
			if (difficulty == Difficulty::Insane || difficulty == Difficulty::Hard)
			{
				if (node.child("PointGravity"))
				{
					pointGravity->Init(node.child("PointGravity").attribute("range").as_float(), node.child("PointGravity").attribute("strength").as_float(), offset);
					newObject->AddComponent(pointGravity);
					Physics.AddGravityAffector(pointGravity);
				}
			}
			if (node.child("Health"))
			{
				health->SetHealth(node.child("Health").attribute("maxHealth").as_int());
				newObject->AddComponent(health);
			}

			if (difficulty == Difficulty::Insane || difficulty == Difficulty::Hard)
			{
				if (node.child("Rigidbody"))
				{
					rigidBody->setMass(node.child("Rigidbody").attribute("Mass").as_double());
					newObject->AddComponent(rigidBody);
					Physics.AddRigidbody(rigidBody);
				}
			}
			for (pugi::xml_node child : node.children("GameObject"))
			{
				newObject->AddChild(CreateNewObject<GameObject>(child, id, level));
			}
			//newObject->isActive(node.attribute("active").as_bool());
			newObject->Start();


			return newObject;
		}
		
		
	};

}

#endif //GAME_MANAGER