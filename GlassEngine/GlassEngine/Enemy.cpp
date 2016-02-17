#include "precomp.h"
#include "Enemy.h"
#include "Bullet.h"

void EnemyAI::Start()
{
	GameObject::Start();
	if (!GetComponent<Health>(HealthC))
	{
		auto health = MakeSmartPtr<Health>();
		health->SetHealth(50 * Game.GetDifficulty());
	}
}

void EnemyAI::Update()
{
	transform->Translate(Vec3LEFT * 0.05f * Time.DeltaTime());

	auto rand_ = rand() % 600;
	if (rand_  < 10)
	{
		auto bullet = Game.SpawnBullet(transform->GetPosition(), Vec2d(-10, 0), GetTag());
		if (bullet->GetComponent<Rigidbody>(RigidbodyC))
			bullet->GetComponent<Rigidbody>(RigidbodyC)->IsEnabled(false);
		auto castedBullet = std::static_pointer_cast<Bullet, GameObject>(bullet);
		castedBullet->UsePhysics(false);
		castedBullet->SpriteRef(1);
	}
	if (transform->GetPosition().x < -150)
	{
		Game.IncreaseScore(-10 * Game.GetDifficulty());
		isActive(false);
	}
	GameObject::Update();
}

void EnemyAI::Explode()
{
	Game.SpawnExplosion(transform->GetPosition(), 3);
	HAPI->PlayASound(Game.CurrentLevel()->GetAudioID("LargeExplosion"));
	Game.IncreaseKillCount();
	Game.IncreaseScore(5);
	isActive(false);
}