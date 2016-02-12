#if !defined (GAMEOBJECT)
#define GAMEOBJECT


namespace GlassEngine{
	
	class Rigidbody;
	class Transform;
	class Sprite;
	class SpriteSheet;
	class Component;
	
	class GameObject
	{
	public:
		GameObject();
		GameObject(int id_);
		virtual ~GameObject();

		virtual void Start();
		virtual void Update();
		virtual void FixedUpdate(){};
		virtual void Stop();

		template<typename T>
		std::shared_ptr<T> GetComponent(int id)
		{
			for (auto comp : components)
			{
				if (comp->GetID() == id)
					return std::dynamic_pointer_cast<T>(comp);
			}
			return nullptr;
		}
		void AddComponent(std::shared_ptr<Component> comp);
		bool HasComponent(int id);

		void AddChild(std::shared_ptr<GameObject> child){ children.push_back(child); };
		std::vector<std::shared_ptr<GameObject>>& GetChildren(){ return children; };

		void SetName(const std::string& newName){ name = newName; };

		std::shared_ptr<Transform>& GetTransform() { return transform; };

		void DeleteObject();

		const int GetID() { return id; };
		const bool isActive(){ return active; };
		void isActive(bool active_);


		void SpriteRef(const int ref){ sprite = ref; };
		void SpriteSheetRef(const int ref){ spritesheet = ref; };
		void ColliderRef(const int ref){ collider = ref; };

		int SpriteRef() const { return sprite; };
		int SpriteSheetRef() const { return spritesheet; };
		int ColliderRef() const { return collider; };

		void Collided(const bool col_){ hasCollided = col_; };
		bool Collided() const { return hasCollided; };

		std::string GetName() const { return name; };
	protected :
		std::vector<std::shared_ptr<Component>> components;
		int sprite = -1;
		int spritesheet = -1;
		int collider = -1;
		std::vector<std::shared_ptr<GameObject>> children;
		void UpdateChildren(Vec3d parentPos);

		int id = 0;

		std::shared_ptr<Transform> transform;
		std::string name = "";
		bool active = false;
		bool hasCollided = false;
	};

}
#endif //GAMEOBJECT