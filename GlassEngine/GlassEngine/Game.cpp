#include "precomp.h"
#include "Game.h"
#include "Sprite.h"
#include "SpriteSheet.h"
#include "Transform.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "GameObject.h"
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

	//TODO: Convert levels to shared ptrs
	void GameManager::Update()
	{
		for (std::shared_ptr<GameObject> gos : levels[currentLevel]->GetGameObjects())
		{
			gos->Update();
		}

		//for (auto gos : CurrentLevel().GetGameObjects())
		//{
		//	std::shared_ptr<Transform> t = gos->GetTransform();
		//	Vec3i pos = t->GetPosition();
		//	Vec2i size = gos->GetSprite()->GetSpriteDims();
		//	Renderer.RenderDR(Vec2i(pos.x - 5, pos.y - 5), Vec2i(size.x + 10, size.y + 10));
		//}

		if (Input.GetKeyUp(HK_F5))
		{
			int id = CurrentLevel()->GetID();
			std::shared_ptr<Level> newLevel = loadLevel(CurrentLevel()->GetLevelName());
			if (newLevel != nullptr)
			{
				CurrentLevel()->DeleteLevel();
				levels.pop_back();
				levels.push_back(newLevel);
				Renderer.ClearPrevious();
				Input.Reset();
			}
			else
			{
				Renderer.ResetToPrevious();
				Renderer.ClearPrevious();
			}
		}
		
	}

	void GameManager::Stop()
	{
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
				for (pugi::xml_node player = level.child("Players").child("Player"); player; player = player.next_sibling("Player"))
				{
					newLevel->AddGameObject(CreateNewObject(player, (int)newLevel->GetGameObjects().size()));
				}
				for (pugi::xml_node gameObject = level.child("World").child("GameObject"); gameObject; gameObject = gameObject.next_sibling("GameObject"))
				{
					newLevel->AddGameObject(CreateNewObject(gameObject, (int)newLevel->GetGameObjects().size()));
				}
				//Renderer.AddSprite(level.child("Sprites").attribute("graphic").as_string());
				//for (pugi::xml_node sprite = level.child("Sprites"); sprite; sprite = sprite.next_sibling("Sprite"))
				//{
				//	newLevel.AddGameObject(CreateNewObject(sprite));
				//}
				levelLoaded = true;
				Renderer.Render(Background, Vec3i(0, 0, 0));
				return newLevel;
			}
		}
		else
		{
			HAPI->UserMessage("Error: 055236. XML Level Load FAILED", "Glass Engine");
		}
		return nullptr;
	}

	std::shared_ptr<GameObject> GameManager::CreateNewObject(pugi::xml_node node, int id)
	{
		std::shared_ptr<GameObject> newObject = std::make_shared<GameObject>(id);
		std::shared_ptr<Transform> transform = std::make_shared<Transform>(newObject);
		std::shared_ptr<Rigidbody> rigidBody = std::make_shared<Rigidbody>(newObject);
		std::shared_ptr<Collider> collider = std::make_shared<Collider>(newObject);
		std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(newObject);
		std::shared_ptr<SpriteSheet> spriteSheet = std::make_shared<SpriteSheet>(newObject);

		transform->SetPosition(Vec3i(node.child("Transform").attribute("positionX").as_int(), node.child("Transform").attribute("positionY").as_int(), node.child("Transform").attribute("positionZ").as_int()));
		//rigidBody->setMass(node.child("Rigidbody").attribute("Mass").as_double());
		//collider->Start(Rect(node.child("Collider").attribute("Top").as_double(), node.child("Collider").attribute("Left").as_double(), node.child("Collider").attribute("Right").as_double(), node.child("Collider").attribute("Bottom").as_double()));
		if (node.child("Sprite"))
		{
			sprite->LoadSprite(node.child("Sprite").attribute("graphic").as_string());
		}
		if (node.child("SpriteSheet"))
		{
			spriteSheet->LoadSprite(node.child("SpriteSheet").attribute("graphic").as_string());
			spriteSheet->SetIdvSpriteSize(Vec2i(node.child("SpriteSheet").attribute("sizeX").as_int(), node.child("SpriteSheet").attribute("sizeY").as_int()));
			for (pugi::xml_node anim : node.child("SpriteSheet").children("Animation"))
			{
				spriteSheet->AddAnimation(Animation(anim.attribute("name").as_string(), Vec2i(anim.attribute("start").as_int(), anim.attribute("end").as_int())));
			}
		}
		//rigidBody->IsEnabled(false);
		newObject->AddComponent(transform);
		
		/*if (node.child("Rigidbody"))
		{
			newObject->AddComponent(rigidBody);
			Physics.AddRigidbody(rigidBody);
		}

		if (node.child("Collider"))
		{
			newObject->AddComponent(collider);
			Physics.AddCollider(collider);
		}*/

		if (node.child("Sprite"))
		{
			newObject->AddComponent(sprite);
			Renderer.AddSprite(sprite);
		}
		if (node.child("SpriteSheet"))
		{
			newObject->AddComponent(spriteSheet);
			Renderer.AddSpriteSheet(spriteSheet);
		}

		newObject->Start();
		
		newObject->SetName(node.attribute("name").as_string());

		for (pugi::xml_node child : node.children("GameObject"))
		{
			newObject->AddChild(CreateNewObject(child, id));
		}

		return newObject;
	}
}