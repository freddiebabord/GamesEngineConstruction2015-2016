#include "precomp.h"
#include "EnemyBoss.h"
#include "Bullet.h"

void BossAI::Start()
{
	GameObject::Start();
}

void BossAI::Update()
{
	
	transform->Translate(Vec3LEFT * 0.04f * Time.DeltaTime());

	auto rand_ = rand() % 600;
	if (rand_  < 10)
	{
		auto bullet = Game.SpawnBullet(transform->GetPosition(), Vec2d(-10, 0), GetTag());
		if (bullet->GetComponent<Rigidbody>(RigidbodyC))
			bullet->GetComponent<Rigidbody>(RigidbodyC)->IsEnabled(false);
		auto castedBullet = std::static_pointer_cast<Bullet, GameObject>(bullet);
		castedBullet->UsePhysics(false);
		castedBullet->SpriteRef(6);

		auto spawnPos1 = transform->GetPosition();
		spawnPos1.y -= 50;
		auto bullet1 = Game.SpawnBullet(spawnPos1, Vec2d(-10, 0), GetTag());
		if (bullet1->GetComponent<Rigidbody>(RigidbodyC))
			bullet1->GetComponent<Rigidbody>(RigidbodyC)->IsEnabled(false);
		auto castedBullet1 = std::static_pointer_cast<Bullet, GameObject>(bullet);
		castedBullet1->UsePhysics(false);
		castedBullet1->SpriteRef(6);

		auto spawnPos2 = transform->GetPosition();
		spawnPos2.y += 50;
		auto bullet2 = Game.SpawnBullet(spawnPos2, Vec2d(-10, 0), GetTag());
		if (bullet2->GetComponent<Rigidbody>(RigidbodyC))
			bullet2->GetComponent<Rigidbody>(RigidbodyC)->IsEnabled(false);
		auto castedBullet2 = std::static_pointer_cast<Bullet, GameObject>(bullet);
		castedBullet2->UsePhysics(false);
		castedBullet2->SpriteRef(6);
	}

	GameObject::Update();
	
	if (transform->GetPosition().x < -150)
	{
		Game.IncreaseScore(-90 * Game.GetDifficulty());
		isActive(false);
		ShouldDie = true;
	}

	if (ShouldDie)
		Game.GameOver(gameWon, Game.GetScore());
}

void BossAI::Explode()
{
	EnemyAI::Explode();
	Game.IncreaseScore(50 * Game.GetDifficulty());
	gameWon = true;
	ShouldDie = true;
}