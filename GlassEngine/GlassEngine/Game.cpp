#include "precomp.h"
#include "Game.h"
#include "Sprite.h"
#include "SpriteSheet.h"
#include "Transform.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "PointGravity.h"
#include "GameObject.h"
#include "PhysicsManager.h"
#include "InputManager.h"
#include "RenderManager.h"

namespace GlassEngine{
	

	GameManager *GameManager::instance{ nullptr };

	GameManager &GameManager::Instance()
	{
		if (instance == nullptr)
			instance = new GameManager;
		return *instance;
	}

	void GameManager::Start()
	{
		levels.push_back(Game.loadLevel("Resources/Levels/Demo Level 1.xml"));
	}

	void GameManager::FixedUpdate()
	{
		for (auto g : CurrentLevel()->GetGameObjects())
			g->FixedUpdate();
	}


	void GameManager::Update()
	{
		for (std::shared_ptr<GameObject> gos : levels[currentLevel]->GetGameObjects())
		{
			gos->Update();
		}

		if (Input.GetKeyUp(HK_F5))
		{
			Physics.ClearPhysicsManager();
			Renderer.ClearRenderer();
			int id = CurrentLevel()->GetID();
			std::shared_ptr<Level> newLevel = loadLevel(CurrentLevel()->GetLevelName());
			if (newLevel != nullptr)
			{
				
				CurrentLevel()->DeleteLevel();
				levels.pop_back();
				Renderer.ClearPrevious();
				Physics.ClearPrevious();
				Input.Reset();
				levels.push_back(newLevel);
			}
			else
			{
				Physics.ResetToPrevious();
				Physics.ClearPrevious();
				Renderer.ResetToPrevious();
				Renderer.ClearPrevious();
			}
		}
		
	}

	void GameManager::Stop()
	{
		std::map<std::string, std::shared_ptr<GameObject>>::iterator itr = prefabs.begin();
		if (itr != prefabs.end())
		{
			itr->second->DeleteObject();
		}
		DeleteLevels();
		delete instance;
	}

	void GameManager::DeleteLevels()
	{
		for (auto l : levels)
			l->DeleteLevel();
		while (!levels.empty())
			levels.pop_back();
		levels.clear();
	}

	std::shared_ptr<Level> GameManager::CurrentLevel()
	{
		return Game.levels[currentLevel];
	}

	std::shared_ptr<Level> GameManager::loadLevel(char* name)
	{
		
		levelLoaded = false;
		levelFile = name;
		return loadLevel();
	}

	std::shared_ptr<Level> GameManager::loadLevel()
	{
		
		
		std::shared_ptr<Level> newLevel = std::make_shared<Level>(instance, levelFile);
		pugi::xml_document doc;

		pugi::xml_parse_result result = doc.load_file(levelFile);
		int currentPlayer = 0;
		if (result && !levelLoaded)
		{
			Renderer.ClearRenderer();
			for (pugi::xml_node level = doc.child("level"); level; level = level.next_sibling("level"))
			{
				newLevel->SetID(level.attribute("id").as_int());
				Renderer.AddSprite(level.child("Background").attribute("graphic").as_string());
				for (pugi::xml_node prefab = level.child("Prefabs").child("GameObject"); prefab; prefab = prefab.next_sibling("GameObject"))
				{
					AddPrefab(CreateNewObject(prefab, (int)prefabs.size()), prefab.attribute("name").as_string());
				}
				for (pugi::xml_node player = level.child("Players").child("Player"); player; player = player.next_sibling("Player"))
				{
					newLevel->AddGameObject(CreateNewObject(player, (int)newLevel->GetGameObjects().size()));
				}
				for (pugi::xml_node gameObject = level.child("World").child("GameObject"); gameObject; gameObject = gameObject.next_sibling("GameObject"))
				{
					newLevel->AddGameObject(CreateNewObject(gameObject, (int)newLevel->GetGameObjects().size()));
				}
				levelLoaded = true;
				Renderer.Render(Background, Vec3d(0.0));
				return newLevel;
			}
		}
		else
		{
			HAPI->UserMessage("Error: 055236. XML Level Load FAILED", "Glass Engine");
		}
		return nullptr;
	}

	std::shared_ptr<GameObject> GameManager::Instantiate(std::string gameObjectName)
	{
		std::shared_ptr<GameObject> prefabObj_ = std::make_shared<GameObject>(*prefabs[gameObjectName]);
		CurrentLevel()->AddGameObject(prefabObj_);
		return prefabObj_;
	}

	std::shared_ptr<GameObject> GameManager::Instantiate(std::string gameObjectName, Vec3d pos)
	{
		std::shared_ptr<GameObject> prefabObj_ = std::make_shared<GameObject>(*prefabs[gameObjectName]);
		prefabObj_->GetTransform()->SetPosition(pos);
		CurrentLevel()->AddGameObject(prefabObj_);
		return prefabObj_;
	}

	std::shared_ptr<GameObject> GameManager::CreateNewObject(pugi::xml_node node, int id)
	{
		std::shared_ptr<GameObject> newObject = std::make_shared<GameObject>(id);
		std::shared_ptr<Transform> transform = std::make_shared<Transform>(newObject);
		std::shared_ptr<Rigidbody> rigidBody = std::make_shared<Rigidbody>(newObject);
		std::shared_ptr<Collider> collider = std::make_shared<Collider>(newObject);
		std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(newObject);
		std::shared_ptr<SpriteSheet> spriteSheet = std::make_shared<SpriteSheet>(newObject);
		std::shared_ptr<PointGravity> pointGravity = std::make_shared<PointGravity>(newObject);

		transform->SetPosition(Vec3d(node.child("Transform").attribute("positionX").as_double(), node.child("Transform").attribute("positionY").as_double(), node.child("Transform").attribute("positionZ").as_double()));
		//;
		//collider->Start(Rect(node.child("Collider").attribute("Top").as_double(), node.child("Collider").attribute("Left").as_double(), node.child("Collider").attribute("Right").as_double(), node.child("Collider").attribute("Bottom").as_double()));
		
		Vec2d offset(0.0);
		if (node.child("Sprite"))
		{
			sprite->LoadSprite(node.child("Sprite").attribute("graphic").as_string());
			offset = Vec2d(sprite->GetSpriteDims().x / 2, sprite->GetSpriteDims().y / 2);
			newObject->AddComponent(sprite);
			Renderer.AddSprite(sprite);
		}
		if (node.child("SpriteSheet"))
		{
			spriteSheet->LoadSprite(node.child("SpriteSheet").attribute("graphic").as_string());
			spriteSheet->SetIdvSpriteSize(Vec2i(node.child("SpriteSheet").attribute("sizeX").as_int(), node.child("SpriteSheet").attribute("sizeY").as_int()));
			spriteSheet->SetAnimSpeed(node.child("SpriteSheet").attribute("animationSpeed").as_int());
			for (pugi::xml_node anim : node.child("SpriteSheet").children("Animation"))
			{
				spriteSheet->AddAnimation(Animation(anim.attribute("name").as_string(), Vec2i(anim.attribute("start").as_int(), anim.attribute("end").as_int())));
			}
			offset = Vec2d(spriteSheet->GetIdvSpriteDims().x / 2, spriteSheet->GetIdvSpriteDims().y / 2);
			newObject->AddComponent(spriteSheet);
			Renderer.AddSpriteSheet(spriteSheet);
		}

		if (node.child("PointGravity"))
		{
			pointGravity->Init(node.child("PointGravity").attribute("range").as_float(), node.child("PointGravity").attribute("strength").as_float(), offset);
			newObject->AddComponent(pointGravity);
			Physics.AddGravityAffector(pointGravity);
		}
		//rigidBody->IsEnabled(false);
		newObject->AddComponent(transform);
		
		if (node.child("Rigidbody"))
		{
			rigidBody->setMass(node.child("Rigidbody").attribute("Mass").as_double());
			newObject->AddComponent(rigidBody);
			Physics.AddRigidbody(rigidBody);
		}

		/*if (node.child("Collider"))
		{
			newObject->AddComponent(collider);
			Physics.AddCollider(collider);
		}*/
		
		newObject->SetName(node.attribute("name").as_string());

		for (pugi::xml_node child : node.children("GameObject"))
		{
			newObject->AddChild(CreateNewObject(child, id));
		}
		//newObject->isActive(node.attribute("active").as_bool());
		newObject->Start();
		
		return newObject;
	}
}