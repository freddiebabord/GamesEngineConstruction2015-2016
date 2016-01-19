#if !defined (PHYSICS_MANAGER)
#define PHYSICS_MANAGER

#include "Manager.h"

namespace GlassEngine{

#define Physics PhysicsManager::Instance()

	class Collider;
	class Rigidbody;

	class PhysicsManager : public Manager
	{
	public:
		static PhysicsManager &Instance();
		~PhysicsManager(){};

		void Start() override;
		void Update() override;
		void FixedUpdate() override;
		void Stop() override;

		void AddCollider(std::shared_ptr<Collider> collider);
		void AddRigidbody(std::shared_ptr<Rigidbody> rigidbody);

	protected:
		PhysicsManager(){};

	private:


	private:
		static PhysicsManager *instance;
		std::vector<std::shared_ptr<Collider>> colliders;
		std::vector<std::shared_ptr<Rigidbody>> rigidbodies;
	};


}
#endif //PHYSICS_MANAGER
