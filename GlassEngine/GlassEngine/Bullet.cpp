#include "precomp.h"
#include "Bullet.h"

void Bullet::OnEnable()
{
	timeToDie = Time.CurrentTime() + amountOfTimeAlive;
}

void Bullet::Update()
{
	if (timeToDie < Time.CurrentTime())
		Explode();
	if (!usePhysics)
		transform->Translate((force/5) * 0.5f * Time.DeltaTime());
	GameObject::Update();
}

void Bullet::OnCollisionEnter(SmartPtr<GameObject> gameObject)
{
	if (gameObject->isActive())
	{
		if (tag == Tag::EnemyTag)
			Game.IncreaseScore();

		if (tag != gameObject->GetTag())
		{
			if (gameObject->GetComponent<Health>(HealthC))
				gameObject->GetComponent<Health>(HealthC)->AddToHealth(-strength);
			Explode();
			HAPI->PlayASound(Game.CurrentLevel()->GetAudioID("SmallExplosion"));
		}
		else
			isActive(false);
		GameObject::OnCollisionEnter(gameObject);
	}
}