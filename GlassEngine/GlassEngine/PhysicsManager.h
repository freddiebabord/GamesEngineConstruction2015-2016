#if !defined (PHYSICS_MANAGER)
#define PHYSICS_MANAGER

#include "Manager.h"

namespace GlassEngine{

#define Physics PhysicsManager::Instance()

	class Collider;
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

		void AddCollider(std::shared_ptr<Collider> collider);
		void AddRigidbody(std::shared_ptr<Rigidbody> rigidbody);
		void AddGravityAffector(std::shared_ptr<PointGravity> grav_);
		std::vector<std::shared_ptr<Rigidbody>>& GetRigidbodies(){ return rigidbodies; };
		void ClearPhysicsManager();
		void ResetToPrevious();
		void ClearPrevious();

	protected:
		PhysicsManager(){};


	private:
		static PhysicsManager *instance;
		std::vector<std::shared_ptr<Collider>> colliders;
		std::vector<std::shared_ptr<Rigidbody>> rigidbodies;
		std::vector<std::shared_ptr<PointGravity>> gravityAffectors;

		std::vector<std::shared_ptr<Collider>> previousColliders;
		std::vector<std::shared_ptr<Rigidbody>> previousRigidbodies;
		std::vector<std::shared_ptr<PointGravity>> previousGravityAffectors;
	};


}
#endif //PHYSICS_MANAGER
