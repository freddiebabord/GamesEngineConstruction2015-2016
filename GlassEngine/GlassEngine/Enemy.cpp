#include "Enemy.h"

void EnemyAI::Update()
{
	transform->Translate(Vec3LEFT * 0.05f * Time.DeltaTime());

	auto rand_ = rand() % 600;
	if (rand_ / 4 < 1)
		Game.SpawnBullet(transform->GetPosition(), Vec2d(-10, 0), GetTag());
	GameObject::Update();
}
