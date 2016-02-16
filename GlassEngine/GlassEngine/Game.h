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

namespace GlassEngine{

#define Game GameManager::Instance()

	class CPlayer;

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
		
		SmartPtr<Level> loadLevel(char* name);
		SmartPtr<Level> CurrentLevel();
		
		void SpawnBullet(Vec3d position, Vec2d force, Tag tag_);
		void SpawnExplosion(Vec3d position);
		
		void Destroy(SmartPtr<GameObject> gameobject);
		void ReloadGame();

	protected:
		GameManager(){};

	private:
		
		SmartPtr<Level> loadLevel();
		void AddPrefab(SmartPtr<GameObject> prefab_, std::string key_){ prefabs[key_] = prefab_; };
		void CreateSprite(pugi::xml_node node);
		void CreateUISprite(pugi::xml_node node);
		void CreateSpriteSheet(pugi::xml_node node);
		void CreateBulletPool(int poolSize, int spriteID, pugi::xml_node node, SmartPtr<Level> currentLevel);
		void CreateExplosionPool(int poolSize, int spriteSheetID, SmartPtr<Level> currentLevel, pugi::xml_node node);
	private:
		std::vector<SmartPtr<Level>> levels;
		std::map<std::string, SmartPtr<GameObject>> prefabs;
		static GameManager *instance;
		char* levelFile = "";
		bool levelLoaded = false;
		int currentLevel = 0;
		std::vector<SmartPtr<GameObject>> bullets;
		std::vector<SmartPtr<GameObject>> explosions;

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

			for (pugi::xml_node child : node.children("GameObject"))
			{
				newObject->AddChild(CreateNewObject<GameObject>(child, id, level));
			}
			//newObject->isActive(node.attribute("active").as_bool());
			newObject->Start();


			return newObject;
		}


		void CreateNewUIObject(pugi::xml_node node, int id)
		{
			SmartPtr<Text> text = MakeSmartPtr<Text>();
			SmartPtr<Button> button = MakeSmartPtr<Button>();

			if (node.child("Text"))
			{
				text->Position(Vec2d(node.child("Text").attribute("PositionX").as_double(), node.child("Text").attribute("PositionY").as_double()));
				text->Content(node.child("Text").attribute("Content").as_string());
				Font textFont;
				textFont.antiAliasing = node.child("Text").child("Font").attribute("AntiAliasing").as_bool();
				textFont.fontSize = node.child("Text").child("Font").attribute("Size").as_int();
				textFont.fontWeight = node.child("Text").child("Font").attribute("Weight").as_int();
				textFont.fontColour = HAPI_TColour(node.child("Text").child("Text").attribute("ColourR").as_int(),
					node.child("Text").child("Font").attribute("ColourG").as_int(),
					node.child("Text").child("Font").attribute("ColourB").as_int());
				text->SetFont(textFont);
				text->SetObjectType(uiObjectType::UI_Text);
				UI.AddUIElement(text);
			}
			if (node.child("Button"))
			{
				auto currentNode = node.child("Button");
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
					text->SetObjectType(uiObjectType::UI_Text);
					button->SetText(text);
				}
				if (currentNode.child("UISprite"))
				{
					button->SetUISprite(currentNode.child("UISprite").attribute("id").as_int());
				}
				button->SetObjectType(uiObjectType::UI_Button);
				UI.AddUIElement(button);
			}
		}
	};

}

#endif //GAME_MANAGER