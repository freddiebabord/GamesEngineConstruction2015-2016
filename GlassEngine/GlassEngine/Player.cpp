#include "precomp.h"
#include "Player.h"
#include "Bullet.h"

void Player::Start()
{
	GameObject::Start();
	respawnPosition = transform->GetPosition();
}


void Player::Update()
{
	if (active)
	{
		auto rigidbody = GetComponent<Rigidbody>(RigidbodyC);
		GetComponent<Animation>(AnimationC)->SetCurrentAnimation("idle");
		if (Input.GetButtonUp(HK_DIGITAL_X, id) || Input.GetKeyUp('R'))
		{
			if (rigidbody)
				rigidbody->SetVelocity(Vec3d(0.0, 0.0, 0.0));
		}
		if (Input.GetAxis(HK_ANALOGUE_LEFT_THUMB_X, id) > HK_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			GetComponent<Animation>(AnimationC)->SetCurrentAnimation("moveFwd");
			if (!simplifiedlControlScheme)
				rigidbody->AddForce(Vec2d(0.025, 0.0));
			else
				transform->Translate(Vec3RIGHT * 0.25 * Time.DeltaTime());
		}
		else if (Input.GetAxis(HK_ANALOGUE_LEFT_THUMB_X, id) < -HK_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			GetComponent<Animation>(AnimationC)->SetCurrentAnimation("moveBack");
			if (!simplifiedlControlScheme)
				rigidbody->AddForce(Vec2d(-0.025, 0.0));
			else
				transform->Translate(Vec3LEFT * 0.25 * Time.DeltaTime());
		}
		if (Input.GetAxis(HK_ANALOGUE_LEFT_THUMB_Y, id) > HK_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			GetComponent<Animation>(AnimationC)->SetCurrentAnimation("moveRight");
			if (!simplifiedlControlScheme)
				rigidbody->AddForce(Vec2d(0.0, 0.025));
			else
				transform->Translate(Vec3DOWN * 0.25 * Time.DeltaTime());
		}
		else if (Input.GetAxis(HK_ANALOGUE_LEFT_THUMB_Y, id) < -HK_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			GetComponent<Animation>(AnimationC)->SetCurrentAnimation("moveLeft");
			if (!simplifiedlControlScheme)
				rigidbody->AddForce(Vec2d(0.0, -0.025));
			else
				transform->Translate(Vec3UP * 0.25 * Time.DeltaTime());
		}
		if (Input.GetButton(HK_DIGITAL_A, id))
		{
			if (bulletSpawnRate > 15.0f)
			{
				auto bullet = std::static_pointer_cast<Bullet, GameObject>(Game.SpawnBullet(transform->GetPosition(), Vec2d(20.0, 0.0), tag, 1));
				bullet->SetDamageAmaount(10);
				bullet->SpriteRef(1);
				if(bullet->GetComponent<Rigidbody>(RigidbodyC))
					bullet->GetComponent<Rigidbody>(RigidbodyC)->IsEnabled(true);
				HAPI->PlayASound(Game.CurrentLevel()->GetAudioID("Torpedo"));
				bulletSpawnRate -= 15 * (float)Time.DeltaTime();
			}
		}
		if (Input.GetButton(HK_DIGITAL_B, id))
		{
			if (cascadeQuantity > 20.f)
			{
				cascadeQuantity -= 35.f * (float)Time.DeltaTime();
				auto bullet = std::static_pointer_cast<Bullet, GameObject>(Game.SpawnBullet(transform->GetPosition(), Vec2d(20.0, 0.0), tag, 4));
				bullet->SetDamageAmaount(25);
				bullet->UsePhysics(false);
				if (bullet->GetComponent<Rigidbody>(RigidbodyC))
					bullet->GetComponent<Rigidbody>(RigidbodyC)->IsEnabled(false);
			}
		}
		if (cascadeQuantity > 20.f)
			UI.PlayerCascades[id] = true;
		else
			UI.PlayerCascades[id] = false;

		if (Input.GetButton(HK_DIGITAL_Y, id) && timeSinceReset > maxTimeAfterReset)
		{
			maxTimeAfterReset = Time.CurrentTime() + 50;
			transform->SetPosition(respawnPosition);
			Game.IncreaseScore(-15 * Game.GetDifficulty());
		}
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
				if (Input.GetKeyUp(HK_SPACE))
				{
					auto bullet = std::static_pointer_cast<Bullet, GameObject>(Game.SpawnBullet(transform->GetPosition(), Vec2d(20.0, 0.0), tag, 1));
					bullet->SetDamageAmaount(10);
					bullet->SpriteRef(1);
					if (bullet->GetComponent<Rigidbody>(RigidbodyC))
						bullet->GetComponent<Rigidbody>(RigidbodyC)->IsEnabled(true);
					HAPI->PlayASound(Game.CurrentLevel()->GetAudioID("Torpedo"));
				}
			}
			timeSinceReset += Time.DeltaTime();
			bulletSpawnRate += 1.f *(float)Time.DeltaTime();
			cascadeQuantity += 0.5f * (float)Time.DeltaTime();
			GameObject::Update();
		}

	}
}

void Player::OnCollisionEnter(SmartPtr<GameObject> gameObject)
{
	if (GetComponent<Rigidbody>(RigidbodyC))
		GetComponent<Rigidbody>(RigidbodyC)->Rebound();
	GameObject::OnCollisionEnter(gameObject);
}