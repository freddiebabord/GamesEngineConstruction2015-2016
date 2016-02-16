#include "precomp.h"
#include "PhysicsManager.h"
#include "Rigidbody.h"
#include "SpriteCollider.h"
#include "PointGravity.h"
#include "GameObject.h"
#include "Transform.h"
#include "Game.h"
#include "InputManager.h"

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

		auto &gameObjects = Game.CurrentLevel()->GetGameObjects();
		for (int i = 0; i < gameObjects.size(); ++i)
		{
			auto gos = gameObjects[i];
			if (gos->isActive() && gos->ColliderRef() > 0)
			{
				bool collided = false;
				auto goPos = gos->GetTransform()->GetPosition();
				auto gosColRef = gos->ColliderRef();
				auto gosTag = gos->GetTag();
				for (int j = i; j < gameObjects.size(); ++j)
				{
					auto gos2 = gameObjects[j];
					if (gosTag != gos2->GetTag() && gos2->ColliderRef() > 0)
					{
						if (CheckForCollision(goPos, gosColRef, gos2->GetTransform()->GetPosition(), gos2->ColliderRef()))
						{
							collided = true;
							if (!gos->Collided())
								gos->OnCollisionEnter(gos2);
							else
								gos->OnCollisionStay(gos2);
							if (!gos2->Collided())
								gos2->OnCollisionEnter(gos);
							else
								gos2->OnCollisionStay(gos);
						}
					}
				}
				if (!collided && gos->Collided())
					gos->Collided(false);
			}
		}
		//for (auto gos : gameObjects)
		//{
		//	bool collided = false;
		//	if (gos->isActive() && gos->ColliderRef() > 0)
		//	{
		//		for (auto gos2 : gameObjects)
		//		{
		//			if (gos2->isActive() && gos != gos2 && gos2->ColliderRef() > 0)
		//			{
		//				if (gos->GetTag() != gos2->GetTag())
		//				{
		//					/*if (GetCollider(gos->ColliderRef())->Mask() == GetCollider(gos2->ColliderRef())->Mask())
		//					{*/
		//						if (CheckForCollision(gos->GetTransform()->GetPosition(), gos->ColliderRef(), 
		//							gos2->GetTransform()->GetPosition(), gos2->ColliderRef()))
		//						{
		//							collided = true;
		//							if (!gos->Collided())
		//								gos->OnCollisionEnter(gos2);
		//							if (!gos2->Collided())
		//								gos2->OnCollisionEnter(gos);
		//						}
		//					//}
		//				}
		//			}
		//		}
		//		if (!collided && gos->Collided())
		//			gos->Collided(false);
		//	}
		//}
	}


	void PhysicsManager::Update()
	{
		for (auto r : rigidbodies)
			r->Update();
		for (auto g : gravityAffectors)
			g->Update();
		
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

	void PhysicsManager::AddCollider(SmartPtr<SpriteCollider> collider)
	{
		colliders.push_back(collider);
	}

	void PhysicsManager::AddRigidbody(SmartPtr<Rigidbody> rigidbody)
	{
		rigidbodies.push_back(rigidbody);
	}

	void PhysicsManager::AddGravityAffector(SmartPtr<PointGravity> grav_)
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

	bool PhysicsManager::CheckForCollision(Vec3d position, int colliderID, 
		Vec3d otherPosition, int otherColliderID)
	{

		SmartPtr<SpriteCollider> collider = colliders[colliderID];
		SmartPtr<SpriteCollider> otherCollider = colliders[otherColliderID];

		Rect playerCollisionRect = collider->getBoundingBox();
		Rect enemyCollisionRect = otherCollider->getBoundingBox();

		Vec2i finalPos = Vec2i((int)position.x, (int)position.y);
		finalPos.x -= playerCollisionRect.Width() / 2;
		finalPos.y -= playerCollisionRect.Height() / 2;

		Vec2i otherFinalPos = Vec2i((int)otherPosition.x, (int)otherPosition.y);
		otherFinalPos.x -= enemyCollisionRect.Width() / 2;
		otherFinalPos.y -= enemyCollisionRect.Height() / 2;

		// Find the collision area within player and enemy rectangles
		playerCollisionRect.Translate(finalPos.x, finalPos.y);
		enemyCollisionRect.Translate(otherFinalPos.x, otherFinalPos.y);

		if (!playerCollisionRect.Intersects(enemyCollisionRect))
			return false;

		playerCollisionRect.ClipTo(enemyCollisionRect);
		playerCollisionRect.Translate(-finalPos.x, -finalPos.y);
		enemyCollisionRect.ClipTo(playerCollisionRect);
		enemyCollisionRect.Translate(-otherFinalPos.x, -otherFinalPos.y);

		// Work out this row's starting DWORD and bit offset for the player
		int playerMaskOffset = playerCollisionRect.GetRectDims().left / 32;
		int playerMaskBitOffset = playerCollisionRect.GetRectDims().left % 32;

		// Work out this row's starting DWORD and bit offset for the enemy
		int enemyMaskOffset = enemyCollisionRect.GetRectDims().left / 32;
		int enemyMaskBitOffset = enemyCollisionRect.GetRectDims().left % 32;

		// Note: the collision rects should be same width and height
		for (int y = 0; y<playerCollisionRect.Height(); y++)
		{
			DWORD *playerMaskHere = collider->GetMask() + playerMaskOffset + ((playerCollisionRect.GetRectDims().top + y)*collider->getDwordSize());
			DWORD *enemyMaskHere = otherCollider->GetMask() + enemyMaskOffset + ((enemyCollisionRect.GetRectDims().top + y)*otherCollider->getDwordSize());

			for (int x = 0; x<playerCollisionRect.Width(); x++)
			{
				// Bit test alone
				// Note: there may be a speed up possible by packing bits into DWORDS and doing a DWORD | DWORD
				bool enemyBitHere = (*enemyMaskHere & (1 << enemyMaskBitOffset))>0;
				bool playerBitHere = (*playerMaskHere & (1 << playerMaskBitOffset))>0;
				if (enemyBitHere && playerBitHere)
					return true;

				// Advance through the masks bit by bit
				// Once at end of a DWORD go on to next
				enemyMaskBitOffset++;
				if (enemyMaskBitOffset == 32)
				{
					enemyMaskBitOffset = 0;
					enemyMaskHere++;
				}

				playerMaskBitOffset++;
				if (playerMaskBitOffset == 32)
				{
					playerMaskBitOffset = 0;
					playerMaskHere++;
				}
			}
		}

		return false;
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