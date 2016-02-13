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
#include "Level.h"
#include "Health.h"

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

	void GameManager::CreateBulletPool(int poolSize, int spriteID, pugi::xml_node node, SmartPtr<Level> currentLevel)
	{
		if (bullets.size() != 0)
		{
			for (auto b : bullets)
				b.reset();
			bullets.clear();
		}
		bullets.reserve(sizeof(GameObject) * poolSize);
		for (int i = 0; i < poolSize; ++i)
		{
			auto bullet = CreateNewObject(node, (int)currentLevel->GetGameObjects().size(), currentLevel);
			bullet->isActive(false);
			bullets.push_back(bullet);
			currentLevel->AddGameObject(bullet);
		}
	}

	void GameManager::CreateExplosionPool(int poolSize, int spriteSheetID, SmartPtr<Level> currentLevel, pugi::xml_node node)
	{
		if (explosions.size() != 0)
		{
			for (auto e : explosions)
				e.reset();
			explosions.clear();
		}
		explosions.reserve(sizeof(GameObject) * poolSize);
		for (int i = 0; i < poolSize; ++i)
		{
			auto explosion = CreateNewObject(node, (int)currentLevel->GetGameObjects().size(), currentLevel);
			explosion->isActive(false);
			explosion->SpriteSheetRef(1);
			explosions.push_back(explosion);
			currentLevel->AddGameObject(explosion);
		}
	}

	void GameManager::Destroy(SmartPtr<GameObject> gameobject)
	{
		gameobject->isActive(false);
	}

	void GameManager::FixedUpdate()
	{
		for (auto g : CurrentLevel()->GetGameObjects())
			g->FixedUpdate();
	}


	void GameManager::Update()
	{
		for (SmartPtr<GameObject> gos : levels[currentLevel]->GetGameObjects())
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
			SmartPtr<Level> newLevel = loadLevel(CurrentLevel()->GetLevelName());
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
		std::map<std::string, SmartPtr<GameObject>>::iterator itr = prefabs.begin();
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

	SmartPtr<Level> GameManager::CurrentLevel()
	{
		return Game.levels[currentLevel];
	}

	SmartPtr<Level> GameManager::loadLevel(char* name)
	{
		
		levelLoaded = false;
		levelFile = name;
		return loadLevel();
	}

	SmartPtr<Level> GameManager::loadLevel()
	{
		SmartPtr<Level> newLevel = MakeSmartPtr<Level>(instance, levelFile);
		pugi::xml_document doc;

		pugi::xml_parse_result result = doc.load_file(levelFile);
		int currentPlayer = 0;
		bool bulletPoolCreated = false;
		bool explosionPoolCreated = false;

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
				for (pugi::xml_node player = level.child("Players").child("Player"); player; player = player.next_sibling("Player"))
				{
					auto player_ = CreateNewObject(player, (int)newLevel->GetGameObjects().size() + (int)newLevel->GetUIObjects().size(), newLevel);
					Physics.GetCollider(player_->ColliderRef())->Mask(CollisionMask::Default);
					newLevel->AddGameObject(player_);
				}
				for (pugi::xml_node prefab = level.child("Prefabs").child("GameObject"); prefab; prefab = prefab.next_sibling("GameObject"))
				{
					AddPrefab(CreateNewObject(prefab, (int)prefabs.size(), newLevel), prefab.attribute("name").as_string());
				}
				if (level.child("Bullet")&& !bulletPoolCreated)
				{
					CreateBulletPool(300, level.child("World").child("Bullet").child("Sprite").attribute("id").as_int(), level.child("World").child("Bullet"), newLevel);
					for (auto bullet : bullets)
						bullet->isActive(false);
					bulletPoolCreated = true;
				}
				if (level.child("Explosion") && !explosionPoolCreated)
				{
					CreateExplosionPool(300, level.child("World").child("Explosion").child("SpriteSheet").attribute("id").as_int(), newLevel, level.child("World").child("Explosion"));
					for (auto bullet : bullets)
						bullet->isActive(false);
					explosionPoolCreated = true;
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
		SmartPtr<Sprite> sprite = MakeSmartPtr<Sprite>();
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
			SmartPtr<SpriteCollider> collider = MakeSmartPtr<SpriteCollider>();
			Rect spriteRect = Rect(0, sprite->GetSpriteDims().height, 0, sprite->GetSpriteDims().width);
			collider->GenerateMask(sprite->GetImage(), spriteRect);
			Physics.AddCollider(collider);
		}
	}

	void GameManager::CreateSpriteSheet(pugi::xml_node node)
	{
		SmartPtr<SpriteSheet> spriteSheet = MakeSmartPtr<SpriteSheet>();
		spriteSheet->LoadSprite(node.attribute("graphic").as_string());
		spriteSheet->SetIdvSpriteSize(Vec2i(node.attribute("sizeX").as_int(), node.attribute("sizeY").as_int()));
		spriteSheet->ID(node.attribute("id").as_int());
		Renderer.AddSpriteSheet(spriteSheet);

		SmartPtr<SpriteCollider> collider = MakeSmartPtr<SpriteCollider>();
		Rect spriteRect = Rect(0, spriteSheet->GetIdvSpriteDims().height, 0, spriteSheet->GetIdvSpriteDims().width);
		collider->GenerateMask(spriteSheet->GetImage(), spriteRect);
		Physics.AddCollider(collider);
	}

	SmartPtr<GameObject> GameManager::CreateNewObject(pugi::xml_node node, int id, SmartPtr<Level> level)
	{
		SmartPtr<GameObject> newObject = MakeSmartPtr<GameObject>(id);
		SmartPtr<Transform> transform = MakeSmartPtr<Transform>(newObject);
		SmartPtr<Rigidbody> rigidBody = MakeSmartPtr<Rigidbody>(newObject);
		SmartPtr<Collider> collider = MakeSmartPtr<Collider>(newObject);
		SmartPtr<SpriteCollider> spriteCollider = MakeSmartPtr<SpriteCollider>(newObject);
		SmartPtr<Animation> animation = MakeSmartPtr<Animation>(newObject);
		SmartPtr<PointGravity> pointGravity = MakeSmartPtr<PointGravity>(newObject);
		SmartPtr<Text> text = MakeSmartPtr<Text>(newObject);
		SmartPtr<Health> health = MakeSmartPtr<Health>(newObject);

		newObject->SetName(node.attribute("name").as_string());
		
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
		
		if (node.child("Health"))
		{
			health->SetHealth(node.child("Health").attribute("maxHealth").as_int());
			newObject->AddComponent(health);
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

	void GameManager::SpawnBullet(Vec3d position, Vec2d force)
	{
		for (auto bullet : bullets)
		{
			if (!bullet->isActive())
			{
				CurrentLevel()->GetGameObjects()[bullet->GetID()]->isActive(true);
				bullet->isActive(true);
				bullet->GetTransform()->SetPosition(position);
				bullet->GetComponent<Rigidbody>(RigidbodyC)->IsEnabled(true);
				bullet->GetComponent<Rigidbody>(RigidbodyC)->AddForce(force);
				HAPI->UserMessage("SpwnMe: Bullet", "GlassEngine");
				return;
			}
		}
	}

	void GameManager::SpawnExplosion(Vec3d position)
	{
		for (auto explosion : explosions)
		{
			if (!explosion->isActive())
			{
				explosion->isActive(true);
				CurrentLevel()->GetGameObjects()[explosion->GetID()]->isActive(true);
				explosion->GetTransform()->SetPosition(position);
				Renderer.Render(explosion->SpriteSheetRef(), position, 0);
				HAPI->UserMessage("SpwnMe: Explosion", "GlassEngine");
				return;
			}
		}
	}
}