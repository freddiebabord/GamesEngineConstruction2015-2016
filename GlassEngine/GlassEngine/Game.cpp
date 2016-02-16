#include "precomp.h"
#include "GlassEngine.h"
#include "GamePlayerHeaders.h"

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
		bullets.reserve(sizeof(Bullet) * poolSize);
		for (int i = 0; i < poolSize; ++i)
		{
			SmartPtr<Bullet> bullet = CreateNewObject<Bullet>(node, (int)currentLevel->GetGameObjects().size(), currentLevel);
			bullet->isActive(false);
			bullet->SetName("Bullet");
			Physics.GetCollider(bullet->ColliderRef())->Mask(CollisionMask::PlayerM);
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
			auto explosion = CreateNewObject<GameObject>(node, (int)currentLevel->GetGameObjects().size(), currentLevel);
			explosion->isActive(false);
			if (explosion->GetComponent<Animation>(AnimationC))
			{
				explosion->GetComponent<Animation>(AnimationC)->Loop(false);
				explosion->GetComponent<Animation>(AnimationC)->SetCurrentAnimation("explode");
			}
			Physics.GetCollider(explosion->ColliderRef())->Mask(CollisionMask::None);
			explosion->SetName("Explosion");
			explosion->ColliderRef(-1);
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
			ReloadGame();
		}
		
	}

	void GameManager::ReloadGame()
	{
		Physics.ClearPhysicsManager();
		Renderer.ClearRenderer();
		UI.Reset();
		int id = CurrentLevel()->GetID();
		SmartPtr<Level> newLevel = loadLevel(CurrentLevel()->GetLevelName());
		if (newLevel != nullptr)
		{
			CurrentLevel()->DeleteLevel();
			levels.pop_back();
			Renderer.ClearPrevious();
			Physics.ClearPrevious();
			Input.Reset();
			UI.ConfirmReset();
			levels.push_back(newLevel);
			std::string test = levels.back()->GetAudioTrack();
			HAPI->PlayStreamedMedia(levels.back()->GetAudioTrack());
		}
		else
		{
			Physics.ResetToPrevious();
			Physics.ClearPrevious();
			Renderer.ResetToPrevious();
			Renderer.ClearPrevious();
			UI.RevertReset();
		}
	}

	void GameManager::Stop()
	{
		std::map<std::string, SmartPtr<GameObject>>::iterator itr = prefabs.begin();
		if (itr != prefabs.end())
		{
			itr->second->Destory();
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
					auto player_ = CreateNewObject<Player>(player, (int)newLevel->GetGameObjects().size(), newLevel);
					Physics.GetCollider(player_->ColliderRef())->Mask(CollisionMask::PlayerM);
					player_->SetTag(Tag::PlayerTag);
					newLevel->AddGameObject(player_);
				}
				for (pugi::xml_node ai = level.child("AIs").child("AI"); ai; ai = ai.next_sibling("AI"))
				{
					auto ai_ = CreateNewObject<EnemyAI>(ai, (int)newLevel->GetGameObjects().size(), newLevel);
					Physics.GetCollider(ai_->ColliderRef())->Mask(CollisionMask::Enemy);
					ai_->isActive(true);
					ai_->SetTag(Tag::EnemyTag);
					newLevel->AddGameObject(ai_);
				}
				for (pugi::xml_node prefab = level.child("Prefabs").child("GameObject"); prefab; prefab = prefab.next_sibling("GameObject"))
				{
					AddPrefab(CreateNewObject<GameObject>(prefab, (int)prefabs.size(), newLevel), prefab.attribute("name").as_string());
				}
				if (level.child("Bullet")&& !bulletPoolCreated)
				{
					CreateBulletPool(300, level.child("Bullet").child("Sprite").attribute("id").as_int(), level.child("Bullet"), newLevel);
					bulletPoolCreated = true;
				}
				if (level.child("Explosion") && !explosionPoolCreated)
				{
					CreateExplosionPool(300, level.child("Explosion").child("SpriteSheet").attribute("id").as_int(), newLevel, level.child("Explosion"));
					explosionPoolCreated = true;
				}
				for (pugi::xml_node gameObject = level.child("World").child("GameObject"); gameObject; gameObject = gameObject.next_sibling("GameObject"))
				{
					newLevel->AddGameObject(CreateNewObject<GameObject>(gameObject, (int)newLevel->GetGameObjects().size(), newLevel));
				}
				for (pugi::xml_node uiObject = level.child("UI").child("UISprites").child("UISprite"); uiObject; uiObject = uiObject.next_sibling("UISprite"))
				{
					SmartPtr<Sprite> uiSprite = MakeSmartPtr<Sprite>();
					CreateUISprite(uiObject);
					Renderer.AddUISprite(uiSprite);
				}
				for (pugi::xml_node uiObject = level.child("UI").child("UIObjects").child("UIObject"); uiObject; uiObject = uiObject.next_sibling("UIObject"))
				{
					CreateNewUIObject(uiObject, (int)UI.GetUIObjects().size());
				}
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

	void GameManager::CreateUISprite(pugi::xml_node node)
	{
		SmartPtr<Sprite> sprite = MakeSmartPtr<Sprite>();
		sprite->LoadSprite(node.attribute("graphic").as_string());
		sprite->ID(node.attribute("id").as_int());
		Renderer.AddUISprite(sprite);
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

	void GameManager::SpawnBullet(Vec3d position, Vec2d force, Tag tag_)
	{
		for (auto bullet : bullets)
		{
			if (!bullet->isActive())
			{
				CurrentLevel()->GetGameObjects()[bullet->GetID()]->isActive(true);
				bullet->isActive(true);
				bullet->GetTransform()->SetPosition(position);
				bullet->SetTag(tag_);
				bullet->GetComponent<Rigidbody>(RigidbodyC)->IsEnabled(true);
				bullet->GetComponent<Rigidbody>(RigidbodyC)->ResetVelocity();
				bullet->GetComponent<Rigidbody>(RigidbodyC)->AddForce(force);
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
				if (explosion->GetComponent<Animation>(AnimationC))
					explosion->GetComponent<Animation>(AnimationC)->IsEnabled(true);
				return;
			}
		}
	}
}