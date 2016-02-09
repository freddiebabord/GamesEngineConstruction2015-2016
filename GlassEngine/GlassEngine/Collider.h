#if !defined (COLLIDER)
#define COLLIDER

#include "precomp.h"
#include "Component.h"

namespace GlassEngine{

	class GameObject;

	class Collider : public Component
	{
	public:
		Collider(std::shared_ptr<GameObject> parentObj_) : Component(parentObj_){ id = ColliderC; };
		Collider() : Component() { id = ColliderC; };
		~Collider(){};

		void Start() override {};
		void Start(Rect rect_){ rect = rect_; };
		void Update()override {};
		void Stop()override {};

		bool useCollision() const { return collision; };
		void useCollision(bool collision_){ collision = collision_; };


		Rect GetColliderRect() const { return rect; };

	private:
		Rect rect;
		bool collision = true;
	};

}

#endif