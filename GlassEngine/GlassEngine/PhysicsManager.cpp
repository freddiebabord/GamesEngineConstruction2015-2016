#include "precomp.h"
#include "PhysicsManager.h"
#include "Rigidbody.h"
#include "SpriteCollider.h"
#include "PointGravity.h"
#include "GameObject.h"

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
	
	void PhysicsManager::FixedUpdate()
	{
		for (auto r : rigidbodies)
			r->FixedUpdate();
		for (auto g : gravityAffectors)
			g->FixedUpdate();
	}


	void PhysicsManager::Update()
	{
		/*for (int i = 0; i < colliders.size(); i++)
		{
			for (int j = 0; j < colliders.size(); j++)
			{
				if (colliders[i] != colliders[j] && colliders[i]->useCollision() && colliders[j]->useCollision())
				{
					if (colliders[i]->GetColliderRect().CheckState(colliders[j]->GetColliderRect()) != RectStates::Separated)
					{
						if (i < rigidbodies.size())
							rigidbodies[i]->hasCollided(true);
					}
				}
			}
		}

		for (const auto r : rigidbodies)
		{
			if (r->IsEnabled())
				r->Update();
		}*/
		for (auto r : rigidbodies)
			r->Update();
		for (auto g : gravityAffectors)
			g->Update();
		for (auto c : colliders)
		{
			for (auto oc : colliders)
			{
				if (oc != c)
				{
					if (c->CheckCollider(oc))
					{
						c->GetParent()->Collided(true);
						HAPI->DebugText("Ive collided with something");
					}
				}
			}
		}
	}

	void PhysicsManager::Stop()
	{
		for (auto c : colliders)
			c = nullptr;
		for (auto r : rigidbodies)
			r = nullptr;
		for (auto g : gravityAffectors)
			g = nullptr;

		colliders.clear();
		rigidbodies.clear();
		gravityAffectors.clear();
		//Delete the instance of the rendering manager
		delete instance;
	}

	

	void PhysicsManager::AddCollider(std::shared_ptr<SpriteCollider> collider)
	{
		colliders.push_back(collider);
	}

	void PhysicsManager::AddRigidbody(std::shared_ptr<Rigidbody> rigidbody)
	{
		rigidbodies.push_back(rigidbody);
	}

	void PhysicsManager::AddGravityAffector(std::shared_ptr<PointGravity> grav_)
	{
		gravityAffectors.push_back(grav_);
	}

	void PhysicsManager::Reset()
	{
		for (auto c : colliders)
			c = nullptr;
		for (auto r : rigidbodies)
			r = nullptr;
		for (auto g : gravityAffectors)
			g = nullptr;

		colliders.clear();
		gravityAffectors.clear();
		rigidbodies.clear();
	}

	//Clear the buffers that store sprites in the renderer
	void PhysicsManager::ClearPhysicsManager()
	{
		previousColliders = colliders;
		previousGravityAffectors = gravityAffectors;
		previousRigidbodies = rigidbodies;
		rigidbodies.clear();
		gravityAffectors.clear();
		colliders.clear();
	}

	//Reset the live sprite buffers to what they were before a reload occured
	void PhysicsManager::ResetToPrevious()
	{
		colliders = previousColliders;
		gravityAffectors = previousGravityAffectors;
		rigidbodies = previousRigidbodies;
		previousColliders.clear();
		previousGravityAffectors.clear();
		previousRigidbodies.clear();
	}

	//Clear the previous buffer of sprites - successful reload of levels
	void PhysicsManager::ClearPrevious()
	{
		previousColliders.clear();
		previousGravityAffectors.clear();
		previousRigidbodies.clear();
	}

}