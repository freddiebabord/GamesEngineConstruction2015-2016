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
		SmartPtr<MainMenu> menu = MakeSmartPtr<MainMenu>();
		menu->OnStart();
		levels.push_back(menu);
		//HAPI->PlayStreamedMedia(levels.back()->GetAudioTrack());
	}

	void GameManager::AddLevel(SmartPtr<Level> newLevel)
	{
		levels.push_back(newLevel);
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

		if (Input.GetKeyUp(HK_F6))
		{
			difficulty = Difficulty::Easy;
			ReloadGame();
		}
		if (Input.GetKeyUp(HK_F7))
		{
			difficulty = Difficulty::Normal;
			ReloadGame();
		}
		if (Input.GetKeyUp(HK_F8))
		{
			difficulty = Difficulty::Hard; 
			ReloadGame();
		}
		if (Input.GetKeyUp(HK_F9))
		{
			difficulty = Difficulty::Insane; 
			ReloadGame();
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

	void GameManager::ReloadGame()
	{
		UI.loadNewLevel = false;
		Physics.ClearPhysicsManager();
		Renderer.ClearRenderer();
		UI.Reset();
		SmartPtr<Level> newLevel = loadLevel(CurrentLevel()->GetLevelName());
		if (newLevel != nullptr)
		{
			CurrentLevel()->DeleteLevel();
			DeleteLevels();
			Renderer.ClearPrevious();
			Physics.ClearPrevious();
			Input.Reset();
			UI.ConfirmReset();
			enemiesKilled = 0;
			currentGroupScore = 0;
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

	void GameManager::LoadNewLevel (char* levelFileName)
	{
		UI.Reset();
		Renderer.ClearRenderer();
		Physics.Reset();
		Input.Reset();
		auto level = loadLevel(levelFileName);
		if (level != nullptr)
		{
			UI.loadNewLevel = false;
			DeleteLevels();
			UI.ConfirmReset();
			Renderer.ClearPrevious();
			Physics.ClearPrevious();
			AddLevel(level);
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

	SmartPtr<Level> GameManager::loadLevel(char* levelFileName)
	{
		levelLoaded = false;

		SmartPtr<Level> newLevel = MakeSmartPtr<Level>(instance, levelFileName);
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_file(levelFileName);
		int currentPlayer = 0;
		bool bulletPoolCreated = false;
		bool explosionPoolCreated = false;

		if (result && !levelLoaded)
		{
			Renderer.ClearRenderer();
			pugi::xml_node level = doc.child("level");
			
			newLevel->SetAudioTrack(level.attribute("backgroundAudio").as_string());
			HAPI->PlayStreamedMedia(newLevel->GetAudioTrack());
			newLevel->SetID(level.attribute("id").as_int());
			Renderer.AddSprite(level.child("Background").attribute("graphic").as_string());
				
			/*switch (level.attribute("difficulty").as_int())
			{
			case 0:
				difficulty = Difficulty::Easy;
				break;
			case 1:
				difficulty = Difficulty::Normal;
				break;
			case 2:
				difficulty = Difficulty::Hard;
				break;
			case 3:
				difficulty = Difficulty::Insane;
				break;
			default:
				break;
			}*/

			for (pugi::xml_node spriteSheetNode = level.child("SpriteSheets").child("SpriteSheet"); spriteSheetNode; spriteSheetNode = spriteSheetNode.next_sibling("SpriteSheet"))
			{
				CreateSpriteSheet(spriteSheetNode);
			}
			for (pugi::xml_node spriteNode = level.child("Sprites").child("Sprite"); spriteNode; spriteNode = spriteNode.next_sibling("Sprite"))
			{
				CreateSprite(spriteNode);
			}
			for (pugi::xml_node soundNode = level.child("Sounds").child("Sound"); soundNode; soundNode = soundNode.next_sibling("Sound"))
			{
				newLevel->AddAudio(soundNode.attribute("name").as_string(), soundNode.attribute("path").as_string());
			}
			for (pugi::xml_node player = level.child("Players").child("Player"); player; player = player.next_sibling("Player"))
			{
				auto player_ = CreateNewObject<Player>(player, (int)newLevel->GetGameObjects().size(), newLevel);
				Physics.GetCollider(player_->ColliderRef())->Mask(CollisionMask::PlayerM);
				player_->SetTag(Tag::PlayerTag);
				if (difficulty > 2)
					player_->SimplifiedControl(false);
				else
					player_->SimplifiedControl(true);
				player_->GetComponent<Health>(HealthC)->SetHealth(player_->GetComponent<Health>(HealthC)->GetHealth() / difficulty);
				newLevel->AddGameObject(player_);
			}
			for (pugi::xml_node ai = level.child("AIs").child("AI"); ai; ai = ai.next_sibling("AI"))
			{
				auto ai_ = CreateNewObject<EnemyAI>(ai, (int)newLevel->GetGameObjects().size(), newLevel);
				Physics.GetCollider(ai_->ColliderRef())->Mask(CollisionMask::Enemy);
				ai_->isActive(true);
				ai_->SetTag(Tag::EnemyTag);
				ai_->GetComponent<Health>(HealthC)->SetHealth(ai_->GetComponent<Health>(HealthC)->GetHealth() * difficulty);
				newLevel->AddGameObject(ai_);
			}
			for (pugi::xml_node bossAI = level.child("AIs").child("BossAI"); bossAI; bossAI = bossAI.next_sibling("BossAI"))
			{
				auto ai_ = CreateNewObject<BossAI>(bossAI, (int)newLevel->GetGameObjects().size(), newLevel);
				Physics.GetCollider(ai_->ColliderRef())->Mask(CollisionMask::Enemy);
				ai_->isActive(true);
				ai_->SetTag(Tag::EnemyTag);
				ai_->GetComponent<Health>(HealthC)->SetHealth(ai_->GetComponent<Health>(HealthC)->GetHealth() * difficulty);
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

	SmartPtr<GameObject> GameManager::SpawnBullet(Vec3d position, Vec2d force, Tag tag_, int bulletID)
	{
		for (auto bullet : bullets)
		{
			if (!bullet->isActive())
			{
				bullet->GetTransform()->SetPosition(position);
				bullet->SetTag(tag_);
				CurrentLevel()->GetGameObjects()[bullet->GetID()]->isActive(true);
				bullet->isActive(true);
				bullet->SpriteRef(bulletID);
				auto castedBullet = std::static_pointer_cast<Bullet, GameObject>(bullet);
				castedBullet->SetForce(Vec3d(force.x, force.y, 0.0));
				if (difficulty <= 2)
					castedBullet->UsePhysics(false);
				else if (tag_ == Tag::PlayerTag)
					castedBullet->UsePhysics(true);
				auto rigidbody = bullet->GetComponent<Rigidbody>(RigidbodyC);
				if (rigidbody)
				{
					rigidbody->IsEnabled(true);
					rigidbody->ResetVelocity();
					rigidbody->AddForce(force);
				}
				return bullet;
			}
		}

		return nullptr;
	}

	void GameManager::SpawnExplosion(Vec3d position, int explosionSpriteID)
	{
		for (auto explosion : explosions)
		{
			if (!explosion->isActive())
			{
				explosion->isActive(true);
				CurrentLevel()->GetGameObjects()[explosion->GetID()]->isActive(true);
				explosion->GetTransform()->SetPosition(position);
				explosion->SpriteSheetRef(explosionSpriteID);
				if (explosion->GetComponent<Animation>(AnimationC))
					explosion->GetComponent<Animation>(AnimationC)->IsEnabled(true);
				return;
			}
		}
	}

	void GameManager::CreateNewUIObject(pugi::xml_node node, int id)
	{
		SmartPtr<Text> text = MakeSmartPtr<Text>();
		SmartPtr<Button> button = MakeSmartPtr<Button>();
		SmartPtr<UISprite> sprite = MakeSmartPtr<UISprite>();

		if (node.child("Text"))
		{
			text->Position(Vec2d(node.child("Text").attribute("PositionX").as_double(), node.child("Text").attribute("PositionY").as_double()));
			text->Content(node.child("Text").attribute("Content").as_string());
			Font textFont;
			textFont.antiAliasing = node.child("Text").child("Font").attribute("AntiAliasing").as_bool();
			textFont.fontSize = node.child("Text").child("Font").attribute("Size").as_int();
			textFont.fontWeight = node.child("Text").child("Font").attribute("Weight").as_int();
			textFont.fontColour = HAPI_TColour(node.child("Text").child("Font").attribute("ColourR").as_int(),
				node.child("Text").child("Font").attribute("ColourG").as_int(),
				node.child("Text").child("Font").attribute("ColourB").as_int());
			text->SetFont(textFont);
			text->SetName(node.child("Text").attribute("name").as_string());
			UI.AddUIElement(text);
		}
		if (node.child("Button"))
		{
			auto currentNode = node.child("Button");
			button->SetName(currentNode.attribute("Name").as_string());
			button->Position(Vec2d(currentNode.attribute("PositionX").as_double(), currentNode.attribute("PositionY").as_double()));
			if (currentNode.child("Text"))
			{
				text->Content(currentNode.child("Text").attribute("Content").as_string());
				Font textFont;
				textFont.antiAliasing = currentNode.child("Text").attribute("AntiAliasing").as_bool();
				textFont.fontSize = currentNode.child("Text").attribute("Size").as_int();
				textFont.fontWeight = currentNode.child("Text").attribute("Weight").as_int();
				textFont.fontColour = HAPI_TColour(currentNode.child("Text").attribute("ColourR").as_int(),
					currentNode.child("Text").attribute("ColourG").as_int(),
					currentNode.child("Text").attribute("ColourB").as_int());
				text->SetFont(textFont);
				text->SetName(currentNode.child("Text").attribute("name").as_string());
				button->SetText(text);
			}
			if (currentNode.child("UISprite"))
			{
				button->SetUISprite(currentNode.child("UISprite").attribute("id").as_int());
			}
			button->CreateButtonCollider();
			UI.AddUIElement(button);
		}
		if (node.child("ImagePanel"))
		{
			sprite->Position(Vec2d(node.child("ImagePanel").attribute("PositionX").as_double(), node.child("ImagePanel").attribute("PositionY").as_double()));
			sprite->SetUISprite(node.child("ImagePanel").attribute("id").as_int());
			sprite->SetName(node.child("ImagePanel").attribute("name").as_string());
		}
	}

	void GameManager::GameOver(bool hasWon, int score)
	{
		gameShouldEnd = true;
	}

	void GameManager::Reset()
	{
		enemiesKilled = 0;
		currentGroupScore = 0;
	}
	
}