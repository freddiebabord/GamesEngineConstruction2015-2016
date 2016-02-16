#include "precomp.h"
#include "Rigidbody.h"
#include "Transform.h"
#include "GameObject.h"
#include "Transform.h"
#include "Time.h"

namespace GlassEngine
{
	void Rigidbody::Start()
	{
		transform = parentObject->GetComponent<Transform>(TransformC);
	}
	void Rigidbody::FixedUpdate()
	{
		if (enabled && !collided)
		{
			transform->Translate(velocity * (const int)mass);
		}
		/*if (gravity && !collided)
		{
		transform->Translate(velocity + Vec3i(0, (int)(9.8 * mass), 0));
		}
		else
		{
		transform->Translate(velocity);
		}*/
	}
	void Rigidbody::Update()
	{
		/*if (gravity && !collided)
		{
			transform->Translate(velocity + Vec3i(0, (int)(9.8 * mass), 0));
		}
		else
		{
			transform->Translate(velocity);
		}*/
	}

	void Rigidbody::ResetVelocity()
	{
		velocity = Vec3d(0.0);
	}

	void Rigidbody::Rebound()
	{
		velocity = Vec3d(-velocity.x, -velocity.y, -velocity.z);
	}
	
};