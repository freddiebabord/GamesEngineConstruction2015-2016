#if !defined (RIGIDBODY)
#define RIGIDBODY

#include "Component.h"

namespace GlassEngine{

	class Transform;
	class GameObject;
	class Rigidbody : public Component
	{
	public:
		Rigidbody(std::shared_ptr<GameObject> parentObj_) : Component(parentObj_){ id = RigidbodyC; };
		~Rigidbody(){};

		void Start() override;
		void Update()override;
		void Stop()override {};
		
		bool useGravity() const { return gravity; };
		void useGravity(bool gravity_){ gravity = gravity_; };

		double getMass() const { return mass; };
		void setMass(double mass_){ mass = mass_; };

		Vec3i GetVelocity() const { return velocity; };
		void SetVelocity(Vec3i velocity_){ velocity = velocity_; };

		bool hasCollided() const { return collided; };
		void hasCollided(bool collided_){ collided = collided_; };

	private:
		bool gravity = true;
		bool collided = false;
		double mass = 1.00;
		std::shared_ptr<Transform> transform;
		Vec3i velocity = Vec3i(0);
	};

}

#endif