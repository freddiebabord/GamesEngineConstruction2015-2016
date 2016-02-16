#include "Player.h"

void Player::Update()
{
	if (active)
	{
		auto rigidbody = GetComponent<Rigidbody>(RigidbodyC);
		GetComponent<Animation>(AnimationC)->SetCurrentAnimation("idle");
		if (Input.GetButtonUp(HK_DIGITAL_X, id) || Input.GetKeyUp('R'))
		{
			rigidbody->SetVelocity(Vec3d(0.0, 0.0, 0.0));
		}
		if (Input.GetAxis(HK_ANALOGUE_LEFT_THUMB_X, this->id) > HK_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			GetComponent<Animation>(AnimationC)->SetCurrentAnimation("moveFwd");
			rigidbody->AddForce(Vec2d(0.025, 0.0));
		}
		else if (Input.GetAxis(HK_ANALOGUE_LEFT_THUMB_X, this->id) < -HK_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			GetComponent<Animation>(AnimationC)->SetCurrentAnimation("moveBack");
			rigidbody->AddForce(Vec2d(-0.025, 0.0));
		}
		if (Input.GetAxis(HK_ANALOGUE_LEFT_THUMB_Y, this->id) > HK_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			GetComponent<Animation>(AnimationC)->SetCurrentAnimation("moveRight");
			rigidbody->AddForce(Vec2d(0.0, 0.025));
		}
		else if (Input.GetAxis(HK_ANALOGUE_LEFT_THUMB_Y, this->id) < -HK_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			GetComponent<Animation>(AnimationC)->SetCurrentAnimation("moveLeft");
			rigidbody->AddForce(Vec2d(0.0, -0.025));
		}
		if (Input.GetButtonUp(HK_DIGITAL_A, this->id) && Time.CurrentTime() > 500)
			Game.SpawnBullet(transform->GetPosition(), Vec2d(20.0, 0.0), tag);
		
		GetChildByName("Shield")->isActive(Input.GetButton(HK_DIGITAL_RIGHT_SHOULDER, id));

		if (id == 0)
		{
			if (Input.GetKey('W') || Input.GetKey('A') || Input.GetKey('S') || Input.GetKey('D'))
			{
				if (Input.GetKey('D')){
					GetComponent<Animation>(AnimationC)->SetCurrentAnimation("moveFwd");
					rigidbody->AddForce(Vec2d(0.025, 0.0));
				}

				else if (Input.GetKey('A')){
					GetComponent<Animation>(AnimationC)->SetCurrentAnimation("moveBack");
					rigidbody->AddForce(Vec2d(-0.025, 0.0));
				}

				if (Input.GetKey('W')){
					GetComponent<Animation>(AnimationC)->SetCurrentAnimation("moveLeft");
					rigidbody->AddForce(Vec2d(0.0, -0.025));
				}

				else if (Input.GetKey('S')){
					GetComponent<Animation>(AnimationC)->SetCurrentAnimation("moveRight");
					rigidbody->AddForce(Vec2d(0.0, 0.025));
				}
				if (Input.GetKeyUp(HK_SPACE) && Time.CurrentTime() > 500)
				{
					Game.SpawnBullet(transform->GetPosition(), Vec2d(20, 0.0), tag);
				}
			}
			
		}
		GameObject::Update();
	}
}

void Player::OnCollisionEnter(SmartPtr<GameObject> gameObject)
{
	if (GetComponent<Rigidbody>(RigidbodyC))
		GetComponent<Rigidbody>(RigidbodyC)->Rebound();
	GameObject::OnCollisionEnter(gameObject);
}