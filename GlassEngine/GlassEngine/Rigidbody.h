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
		void FixedUpdate() override;
		void Stop()override {};
		
		bool useGravity() const { return gravity; };
		void useGravity(bool gravity_){ gravity = gravity_; };

		double getMass() const { return mass; };
		void setMass(double mass_){ mass = mass_; };

		Vec3d GetVelocity() const { return velocity; };
		void SetVelocity(Vec3d velocity_){ velocity = velocity_; };

		bool hasCollided() const { return collided; };
		void hasCollided(bool collided_){ collided = collided_; };

		void AddForce(Vec3d force_)
		{
			velocity += (force_);
		};

		void AddForce(Vec2d force_)
		{
			velocity += Vec3d(force_.x, force_.y, 0);
		};

		void ResetVelocity();

	private:
		bool gravity = true;
		bool collided = false;
		double mass = 1.00;
		std::shared_ptr<Transform> transform;
		Vec3d velocity = Vec3d(0);
	};

}

#endif