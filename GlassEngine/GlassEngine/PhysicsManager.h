#if !defined (PHYSICS_MANAGER)
#define PHYSICS_MANAGER

#include "Manager.h"

namespace GlassEngine{

#define Physics PhysicsManager::Instance()

	class SpriteCollider;
	class Rigidbody;
	class PointGravity;

	class PhysicsManager : public Manager
	{
	public:
		static PhysicsManager &Instance();
		~PhysicsManager(){};

		void Start() override;
		void Update() override;
		void FixedUpdate() override;
		void Stop() override;
		void Reset();

		void AddCollider(SmartPtr<SpriteCollider> collider);
		void AddRigidbody(SmartPtr<Rigidbody> rigidbody);
		void AddGravityAffector(SmartPtr<PointGravity> grav_);
		std::vector<SmartPtr<Rigidbody>>& GetRigidbodies(){ return rigidbodies; };
		SmartPtr<SpriteCollider> GetCollider(int colliderID) const { return colliders[colliderID]; };

		bool CheckForCollision(Vec3d position, int colliderID, 
			Vec3d otherPosition, int otherColliderID);

		void ClearPhysicsManager();
		void ResetToPrevious();
		void ClearPrevious();

	protected:
		PhysicsManager(){};


	private:
		static PhysicsManager *instance;
		std::vector<SmartPtr<SpriteCollider>> colliders;
		std::vector<SmartPtr<Rigidbody>> rigidbodies;
		std::vector<SmartPtr<PointGravity>> gravityAffectors;

		std::vector<SmartPtr<SpriteCollider>> previousColliders;
		std::vector<SmartPtr<Rigidbody>> previousRigidbodies;
		std::vector<SmartPtr<PointGravity>> previousGravityAffectors;
	};


}
#endif //PHYSICS_MANAGER
