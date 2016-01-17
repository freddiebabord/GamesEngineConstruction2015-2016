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
		HAPI->DebugText(name);
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
		transform = std::dynamic_pointer_cast<Transform>(GetComponent(TransformC));
		sprite = std::dynamic_pointer_cast<Sprite>(GetComponent(SpriteC));
		spritesheet = std::dynamic_pointer_cast<SpriteSheet>(GetComponent(SpriteSheetC));
	}

	void GameObject::Update()
	{
		/*for (auto comp : components)
		{
			comp->Update();
		}*/
		if (active)
		{
			if (id < 4)
			{
				if (Input.GetAxis(HK_ANALOGUE_LEFT_THUMB_X, id) > HK_GAMEPAD_LEFT_THUMB_DEADZONE)
				{
					transform->Translate(Vec3i(5, 0, 0));
				}
				if (Input.GetAxis(HK_ANALOGUE_LEFT_THUMB_X, id) < -HK_GAMEPAD_LEFT_THUMB_DEADZONE)
				{
					transform->Translate(Vec3i(-5, 0, 0));
				}
				if (Input.GetAxis(HK_ANALOGUE_LEFT_THUMB_Y, id) > HK_GAMEPAD_LEFT_THUMB_DEADZONE)
				{
					transform->Translate(Vec3i(0, 5, 0));
				}
				if (Input.GetAxis(HK_ANALOGUE_LEFT_THUMB_Y, id) < -HK_GAMEPAD_LEFT_THUMB_DEADZONE)
				{
					transform->Translate(Vec3i(0, -5, 0));
				}
				if (Input.GetKey('W') || Input.GetKey('A') || Input.GetKey('S') || Input.GetKey('D'))
				{
					if (Input.GetKey('W')){
						transform->Translate(Vec3i(0, -5, 0));
					}

					else if (Input.GetKey('S')){
						transform->Translate(Vec3i(0, 5, 0));
					}

					if (Input.GetKey('A')){
						transform->Translate(Vec3i(-5, 0, 0));
					}

					else if (Input.GetKey('D')){
						transform->Translate(Vec3i(5, 0, 0));
					}
				}
			}
		}

		for (auto c : children)
			c->GetTransform()->SetPosition(transform->GetPosition());
	}

	void GameObject::Stop()
	{
		for (auto c : children)
			c->Stop();
		for (auto c : components)
			c->Stop();
	}

	std::shared_ptr<Component> GameObject::GetComponent(int id)
	{
		for (auto comp : components)
		{
			if (comp->GetID() == id)
				return comp;
		}
		return nullptr;
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
		components.clear();
		transform.reset();
		sprite.reset();
		spritesheet.reset();
	}
}