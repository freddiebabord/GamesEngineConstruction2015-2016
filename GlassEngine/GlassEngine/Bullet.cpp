#include "Bullet.h"

void Bullet::OnEnable()
{
	timeToDie = Time.CurrentTime() + amountOfTimeAlive;
}

void Bullet::Update()
{
	if (timeToDie < Time.CurrentTime())
		Explode();
	GameObject::Update();
}

void Bullet::OnCollisionEnter(SmartPtr<GameObject> gameObject)
{
	if (tag != gameObject->GetTag())
	{
		if (gameObject->GetComponent<Health>(HealthC))
			gameObject->GetComponent<Health>(HealthC)->AddToHealth(-strength);
		Explode();
	}
	else
		isActive(false);
	GameObject::OnCollisionEnter(gameObject);
}