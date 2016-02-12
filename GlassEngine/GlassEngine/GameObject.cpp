#include "precomp.h"
#include "Component.h"
#include "Transform.h"
#include "Sprite.h"
#include "SpriteSheet.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "GameObject.h"
#include "InputManager.h"
#include "RenderManager.h"
#include "PhysicsManager.h"
#include "Time.h"
#include "Game.h"
#include "Animation.h"

namespace GlassEngine
{
	GameObject::GameObject()
	{
	}

	GameObject::GameObject(int id_)
	{
		id = id_;
	}

	GameObject::~GameObject()
	{
		DeleteObject();
	}


	void GameObject::Start()
	{
		for (auto c : children)
			c->Start();
		for(auto comp : components)
		{
			comp->Start();
		}
		transform = GetComponent<Transform>(TransformC);
	}

	void GameObject::Update()
	{
		if (active)
		{
			if (id < 4 && GetComponent<Animation>(AnimationC))
			{
				auto rigidbody = GetComponent<Rigidbody>(RigidbodyC);
				GetComponent<Animation>(AnimationC)->SetCurrentAnimation("idle");
				if (Input.GetButtonUp(HK_DIGITAL_X, id) || Input.GetKeyUp('R'))
				{
					rigidbody->SetVelocity(Vec3d(0.0, 0.0, 0.0));
				}
				if (Input.GetAxis(HK_ANALOGUE_LEFT_THUMB_X, id) > HK_GAMEPAD_LEFT_THUMB_DEADZONE)
				{
					GetComponent<Animation>(AnimationC)->SetCurrentAnimation("moveFwd");
					rigidbody->AddForce(Vec2d(0.025, 0.0));
				}
				else if (Input.GetAxis(HK_ANALOGUE_LEFT_THUMB_X, id) < -HK_GAMEPAD_LEFT_THUMB_DEADZONE)
				{
					GetComponent<Animation>(AnimationC)->SetCurrentAnimation("moveBack");
					rigidbody->AddForce(Vec2d(-0.025, 0.0));
				}
				if (Input.GetAxis(HK_ANALOGUE_LEFT_THUMB_Y, id) > HK_GAMEPAD_LEFT_THUMB_DEADZONE)
				{
					GetComponent<Animation>(AnimationC)->SetCurrentAnimation("moveRight");
					rigidbody->AddForce(Vec2d(0.0, 0.025));
				}
				else if (Input.GetAxis(HK_ANALOGUE_LEFT_THUMB_Y, id) < -HK_GAMEPAD_LEFT_THUMB_DEADZONE)
				{
					GetComponent<Animation>(AnimationC)->SetCurrentAnimation("moveLeft");
					rigidbody->AddForce(Vec2d(0.0, -0.025));
				}
				if (Input.GetButtonUp(HK_DIGITAL_A, id))
					Game.SpawnBullet(transform->GetPosition(), Vec2d(5.0, 0.0));
				if (id == 0)
				{
					if (Input.GetKey('W') || Input.GetKey('A') || Input.GetKey('S') || Input.GetKey('D'))
					{
						if (Input.GetKey('W')){
							GetComponent<Animation>(AnimationC)->SetCurrentAnimation("moveFwd");
							rigidbody->AddForce(Vec2d(0.025, 0.0));
						}

						else if (Input.GetKey('S')){
							GetComponent<Animation>(AnimationC)->SetCurrentAnimation("moveBack");
							rigidbody->AddForce(Vec2d(-0.025, 0.0));
						}

						if (Input.GetKey('A')){
							GetComponent<Animation>(AnimationC)->SetCurrentAnimation("moveLeft");
							rigidbody->AddForce(Vec2d(0.0, -0.025));
						}

						else if (Input.GetKey('D')){
							GetComponent<Animation>(AnimationC)->SetCurrentAnimation("moveRight");
							rigidbody->AddForce(Vec2d(0.0, 0.025));
						}
					}
				}
			}
		}

		UpdateChildren(transform->GetPosition());
	}

	void GameObject::UpdateChildren(Vec3d parentPos)
	{
		for (auto c : children)
		{
			c->UpdateChildren(parentPos);
		}
		transform->SetPosition(parentPos);
	}

	void GameObject::Stop()
	{
		for (auto c : children)
			c->Stop();
		for (auto c : components)
			c->Stop();
	}

	void GameObject::AddComponent(std::shared_ptr<Component> comp)
	{
		components.push_back(comp);
	}

	bool GameObject::HasComponent(int id)
	{
		for (auto comp : components)
		{
			if (comp->GetID() == id)
				return true;
		}

		return false;
	}

	void GameObject::isActive(bool active_) 
	{ 
		for (auto c : components)
			c->IsEnabled(active_);
		for (auto child : children)
			child->isActive(active_);
		active = active_; 
	}

	void GameObject::DeleteObject()
	{
		for (auto c : children)
		{
			c->DeleteObject();
			c->Stop();
		}
		children.clear();
		
		transform.reset();
		
		components.clear();
	}
}