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
#include "Health.h"


namespace GlassEngine
{
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
		UpdateChildren(transform->GetPosition());

		if (GetComponent<Health>(HealthC))
		{
			if (GetComponent<Health>(HealthC)->GetHealth() < 0)
				Explode();
		}

	}

	SmartPtr<GameObject> GameObject::GetChildByName(std::string name)
	{
		for (auto child : children)
			if (child->GetName() == name)
				return child;

		return nullptr;
	}

	void GameObject::Explode()
	{
		Game.SpawnExplosion(transform->GetPosition());
		isActive(false);
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

	void GameObject::AddComponent(SmartPtr<Component> comp)
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
		if (active_)
			OnEnable();
		active = active_; 
	}

	void GameObject::DeleteObject()
	{
		for (auto c : children)
		{
			c->DeleteObject();
			c->Stop();
		}
		for (auto col : collidingObjects)
			col.reset();
		collidingObjects.clear();
		children.clear();
		transform.reset();
		components.clear();
	}

	void GameObject::OnCollisionEnter(SmartPtr<GameObject> gameObject)
	{
		if (std::find(collidingObjects.begin(), collidingObjects.end(), gameObject) != collidingObjects.end())
			OnCollisionStay(gameObject);
		else
		{
			collidingObjects.push_back(gameObject);
			/*if (gameObject->GetTag() != GetTag())
			{
				if (GetComponent<Health>(HealthC))
					GetComponent <Health>(HealthC)->AddToHealth(-5);
			}*/
		}
			Collided(true);
	}

	void GameObject::OnCollisionStay(SmartPtr<GameObject> gameObject)
	{
	}

	void GameObject::OnCollisionExit(SmartPtr<GameObject> gameObject)
	{
		collidingObjects.erase(std::remove(collidingObjects.begin(), collidingObjects.end(), gameObject));
	}

	void GameObject::Destory()
	{
		DeleteObject();
	}
}