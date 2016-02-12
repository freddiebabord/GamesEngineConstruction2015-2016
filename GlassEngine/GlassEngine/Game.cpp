#include "precomp.h"
#include "Game.h"
#include "Sprite.h"
#include "SpriteSheet.h"
#include "Animation.h"
#include "Transform.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "PointGravity.h"
#include "Text.h"
#include "GameObject.h"
#include "PhysicsManager.h"
#include "InputManager.h"
#include "RenderManager.h"
#include "UIManager.h"
#include "Animator.h"
#include "SpriteCollider.h"

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
		HAPI->PlayStreamedMedia(levels.back()->GetAudioTrack());
	}

	void GameManager::CreateBulletPool(int poolSize, int spriteID, std::shared_ptr<Level> currentLevel)
	{
		bullets.reserve(sizeof(GameObject) * 150);
		for (int i = 0; i < 150; ++i)
		{
			auto bullet = std::make_shared<GameObject>((int)prefabs.size());
			bullet->SpriteRef(spriteID);
			bullet->ColliderRef(spriteID);
			auto transform = std::make_shared<Transform>(bullet);
			transform->SetPosition(Vec3d(0.0));
			auto rigidbody = std::make_shared<Rigidbody>(bullet);
			rigidbody->IsEnabled(false);
			bullet->AddComponent(transform);
			bullet->AddComponent(rigidbody);
			bullet->Start();
			bullet->isActive(false);
			bullets.push_back(bullet);	
		}
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
			if (gos->isActive())
				gos->Update();
		}

		if (Input.GetKeyUp(HK_F5))
		{
			Physics.ClearPhysicsManager();
			Renderer.ClearRenderer();
			//UI.Reset();
			int id = CurrentLevel()->GetID();
			std::shared_ptr<Level> newLevel = loadLevel(CurrentLevel()->GetLevelName());
			if (newLevel != nullptr)
			{
				
				CurrentLevel()->DeleteLevel();
				levels.pop_back();
				Renderer.ClearPrevious();
				Physics.ClearPrevious();
				Input.Reset();
				//UI.ConfirmReset();
				levels.push_back(newLevel);
				std::string test=levels.back()->GetAudioTrack();
				HAPI->PlayStreamedMedia(levels.back()->GetAudioTrack());
			}
			else
			{
				Physics.ResetToPrevious();
				Physics.ClearPrevious();
				Renderer.ResetToPrevious();
				Renderer.ClearPrevious();
				//UI.RevertReset();
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
				newLevel->SetAudioTrack(level.attribute("backgroundAudio").as_string());
				newLevel->SetID(level.attribute("id").as_int());
				Renderer.AddSprite(level.child("Background").attribute("graphic").as_string());
				for (pugi::xml_node spriteSheetNode = level.child("SpriteSheets").child("SpriteSheet"); spriteSheetNode; spriteSheetNode = spriteSheetNode.next_sibling("SpriteSheet"))
				{
					CreateSpriteSheet(spriteSheetNode);
				}
				for (pugi::xml_node spriteNode = level.child("Sprites").child("Sprite"); spriteNode; spriteNode = spriteNode.next_sibling("Sprite"))
				{
					CreateSprite(spriteNode);
				}
				for (pugi::xml_node prefab = level.child("Prefabs").child("GameObject"); prefab; prefab = prefab.next_sibling("GameObject"))
				{
					AddPrefab(CreateNewObject(prefab, (int)prefabs.size(), newLevel), prefab.attribute("name").as_string());
				}
				for (pugi::xml_node player = level.child("Players").child("Player"); player; player = player.next_sibling("Player"))
				{
					newLevel->AddGameObject(CreateNewObject(player, (int)newLevel->GetGameObjects().size() + (int)newLevel->GetUIObjects().size(), newLevel));
				}
				for (pugi::xml_node gameObject = level.child("World").child("GameObject"); gameObject; gameObject = gameObject.next_sibling("GameObject"))
				{
					newLevel->AddGameObject(CreateNewObject(gameObject, (int)newLevel->GetGameObjects().size() + (int)newLevel->GetUIObjects().size(), newLevel));
				}
				/*UI.SetFont(level.child("UI").attribute("Font").as_string(), level.child("UI").attribute("Size").as_int());
				for (pugi::xml_node uio = level.child("UI").child("GameObject"); uio; uio = uio.next_sibling("GameObject"))
				{
					newLevel->AddUIObject(CreateNewObject(uio, (int)newLevel->GetGameObjects().size()+(int)newLevel->GetUIObjects().size()));
					newLevel->GetUIObjects().back()->isActive(true);
				}*/
				levelLoaded = true;
				Renderer.RenderBackground(Background, Vec3d(0.0));
				return newLevel;
			}
		}
		else
		{
			HAPI->UserMessage("Error: 055236. XML Level Load FAILED", "Glass Engine");
		}
		return nullptr;
	}


	void GameManager::CreateSprite(pugi::xml_node node)
	{
		std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>();
		sprite->LoadSprite(node.attribute("graphic").as_string());
		sprite->ID(node.attribute("id").as_int());
		
		if (node.attribute("renderToBackground").as_bool())
		{
			Renderer.RenderToBackground(sprite, Vec3d(node.attribute("positionX").as_double(), node.attribute("positionY").as_double(), 0.0));
			sprite->Renderable(false);
		}
		else
		{
			Renderer.AddSprite(sprite);
			std::shared_ptr<SpriteCollider> collider = std::make_shared<SpriteCollider>();
			Rect spriteRect = Rect(0, sprite->GetSpriteDims().height, 0, sprite->GetSpriteDims().width);
			collider->GenerateMask(sprite->GetImage(), spriteRect);
			Physics.AddCollider(collider);
		}
	}

	void GameManager::CreateSpriteSheet(pugi::xml_node node)
	{
		std::shared_ptr<SpriteSheet> spriteSheet = std::make_shared<SpriteSheet>();
		spriteSheet->LoadSprite(node.attribute("graphic").as_string());
		spriteSheet->SetIdvSpriteSize(Vec2i(node.attribute("sizeX").as_int(), node.attribute("sizeY").as_int()));
		spriteSheet->ID(node.attribute("id").as_int());
		Renderer.AddSpriteSheet(spriteSheet);

		std::shared_ptr<SpriteCollider> collider = std::make_shared<SpriteCollider>();
		Rect spriteRect = Rect(0, spriteSheet->GetIdvSpriteDims().height, 0, spriteSheet->GetIdvSpriteDims().width);
		collider->GenerateMask(spriteSheet->GetImage(), spriteRect);
		Physics.AddCollider(collider);
	}

	std::shared_ptr<GameObject> GameManager::CreateNewObject(pugi::xml_node node, int id, std::shared_ptr<Level> level)
	{
		std::shared_ptr<GameObject> newObject = std::make_shared<GameObject>(id);
		std::shared_ptr<Transform> transform = std::make_shared<Transform>(newObject);
		std::shared_ptr<Rigidbody> rigidBody = std::make_shared<Rigidbody>(newObject);
		std::shared_ptr<Collider> collider = std::make_shared<Collider>(newObject);
		std::shared_ptr<SpriteCollider> spriteCollider = std::make_shared<SpriteCollider>(newObject);
		std::shared_ptr<Animation> animation = std::make_shared<Animation>(newObject);
		std::shared_ptr<PointGravity> pointGravity = std::make_shared<PointGravity>(newObject);
		std::shared_ptr<Text> text = std::make_shared<Text>(newObject);

		newObject->SetName(node.attribute("name").as_string());

		if (newObject->GetName() == "Bullet")
		{
			CreateBulletPool(150, node.child("Sprite").attribute("id").as_int(), level);
		}

		transform->SetPosition(Vec3d(node.child("Transform").attribute("positionX").as_double(), node.child("Transform").attribute("positionY").as_double(), node.child("Transform").attribute("positionZ").as_double()));
		//;
		//collider->Start(Rect(node.child("Collider").attribute("Top").as_double(), node.child("Collider").attribute("Left").as_double(), node.child("Collider").attribute("Right").as_double(), node.child("Collider").attribute("Bottom").as_double()));
		
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
			newObject->AddComponent(animation);
			Animator.AddAnimation(animation);
		}

		if (node.child("PointGravity"))
		{
			pointGravity->Init(node.child("PointGravity").attribute("range").as_float(), node.child("PointGravity").attribute("strength").as_float(), offset);
			newObject->AddComponent(pointGravity);
			Physics.AddGravityAffector(pointGravity);
		}
		
		
		if (node.child("Rigidbody"))
		{
			rigidBody->setMass(node.child("Rigidbody").attribute("Mass").as_double());
			newObject->AddComponent(rigidBody);
			Physics.AddRigidbody(rigidBody);
		}

		if (node.child("Text"))
		{
			text->SetContent(node.child("Text").attribute("Content").as_string());
			newObject->AddComponent(text);
			UI.AddUIElement(text);
		}	

		for (pugi::xml_node child : node.children("GameObject"))
		{
			newObject->AddChild(CreateNewObject(child, id, level));
		}
		//newObject->isActive(node.attribute("active").as_bool());
		newObject->Start();
		
		
		return newObject;
	}

	const void GameManager::SpawnBullet(Vec3d position, Vec2d force)
	{
		for (auto bullet : bullets)
		{
			if (!bullet->isActive())
			{
				bullet->isActive(true);
				bullet->GetTransform()->SetPosition(position);
				bullet->GetComponent<Rigidbody>(RigidbodyC)->IsEnabled(true);
				bullet->GetComponent<Rigidbody>(RigidbodyC)->AddForce(force);
				CurrentLevel()->AddGameObject(bullet);
				return;
			}
		}
	}
}