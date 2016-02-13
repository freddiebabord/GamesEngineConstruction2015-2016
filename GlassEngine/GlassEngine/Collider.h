#if !defined (COLLIDER)
#define COLLIDER

#include "precomp.h"
#include "Component.h"

namespace GlassEngine{

	class GameObject;

	enum CollisionMask
	{
		Default,
		PlayerM,
		Enemy,
		PlayerBullets,
		EnemyBullets,
		Explosions,
		None,
		All
	};

	class Collider : public Component
	{
	public:
		Collider(SmartPtr<GameObject> parentObj_) : Component(parentObj_){ id = ColliderC; };
		Collider() : Component() { id = ColliderC; };
		~Collider(){};

		void Start() override {};
		void Start(Rect rect_){ rect = rect_; };
		void Update()override {};
		void Stop()override {};

		bool useCollision() const { return collision; };
		void useCollision(bool collision_){ collision = collision_; };


		Rect GetColliderRect() const { return rect; };

		void Mask(const CollisionMask mask) { collisionMask = mask; };
		const CollisionMask Mask() const { return collisionMask; };

	protected:
		Rect rect;
		bool collision = true;
		CollisionMask collisionMask = CollisionMask::Default;
	};

}

#endif