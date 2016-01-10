#if !defined (GAMEOBJECT)
#define GAMEOBJECT

#include <typeinfo>

namespace GlassEngine{
	
	class Transform;
	class Sprite;
	class SpriteSheet;
	class Component;
	
	class GameObject
	{
	public:
		GameObject();
		GameObject(int id_);
		~GameObject();

		virtual void Start();
		virtual void Update();
		virtual void Stop();

		std::shared_ptr<Component> GetComponent(int id);

		template<typename T>
		std::shared_ptr<T> GetComponent(T component)
		{
			for (auto comp : components)
			{
				if (type_info(comp) == type_info(component))
					return std::dynamic_pointer_cast<T>(comp);
			}
			return nullptr;
		}
		void AddComponent(std::shared_ptr<Component> comp);
		bool HasComponent(int id);

		void SetName(const std::string& newName){ name = newName; };

		std::shared_ptr<Sprite>& GetSprite() { return sprite; };
		std::shared_ptr<SpriteSheet>& GetSpritesheet() { return spritesheet; };
		std::shared_ptr<Transform>& GetTransform() { return transform; };

		const int GetID() { return id; };
		const bool isActive(){ return active; };
		void isActive(bool active_);

	protected :
		std::vector<std::shared_ptr<Component>> components;
		std::vector<std::shared_ptr<GameObject>> children;

		int id = 0;

		std::shared_ptr<Transform> transform;
		std::shared_ptr<Sprite> sprite;
		std::shared_ptr<SpriteSheet> spritesheet;
		std::string name = "";
		bool active = false;
	};

}
#endif //GAMEOBJECT