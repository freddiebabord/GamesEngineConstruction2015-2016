#if !defined (COMPONENT)
#define COMPONENT


namespace GlassEngine{
	
	class GameObject;

	class Component
	{
	public:
		Component(){};
		Component(std::shared_ptr<GameObject> parentObject_) : parentObject(parentObject_){};
		virtual ~Component() = 0{ parentObject = nullptr; };

		virtual void Start(){};
		virtual void Update(){};
		virtual void FixedUpdate(){};
		virtual void Stop(){};

		int GetID(){ return id; };
		const bool IsEnabled(){ return enabled; };
		void IsEnabled(const bool enabled_){ enabled = enabled_; };
		std::shared_ptr<GameObject> GetParent(){ return parentObject; };

	protected:
		std::shared_ptr<GameObject> parentObject;
		bool enabled = true;
		int id;
	};

#define TransformC 0
#define RigidbodyC 1
#define ColliderC 2
#define SpriteC	3
#define SpriteSheetC 4
#define PointGravityC 5
}
#endif