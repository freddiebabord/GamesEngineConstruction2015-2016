#include "precomp.h"
#include "PhysicsManager.h"
#include "Rigidbody.h"
#include "Collider.h"

namespace GlassEngine{

	PhysicsManager *PhysicsManager::instance{ nullptr };

	PhysicsManager &PhysicsManager::Instance()
	{
		if (instance == nullptr)
			instance = new PhysicsManager;
		return *instance;
	}

	void PhysicsManager::Start()
	{
		
	}

	void PhysicsManager::Update()
	{
		for (int i = 0; i < colliders.size(); i++)
		{
			for (int j = 0; j < colliders.size(); j++)
			{
				if (colliders[i] != colliders[j] && colliders[i]->useCollision() && colliders[j]->useCollision())
				{
					if (colliders[i]->GetColliderRect().CheckState(colliders[j]->GetColliderRect()) != RectStates::Separated)
					{
						rigidbodies[i]->hasCollided(false);
					}
				}
			}
		}

		for (const auto r : rigidbodies)
		{
			if (r->IsEnabled())
				r->Update();
		}
	}

	void PhysicsManager::Stop()
	{
		rigidbodies.clear();

		//Delete the instance of the rendering manager
		delete instance;
	}

	void PhysicsManager::AddCollider(std::shared_ptr<Collider> collider)
	{
		colliders.push_back(collider);
	}

	void PhysicsManager::AddRigidbody(std::shared_ptr<Rigidbody> rigidbody)
	{
		rigidbodies.push_back(rigidbody);
	}

}