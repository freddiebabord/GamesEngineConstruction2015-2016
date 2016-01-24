#include "precomp.h"
#include "PointGravity.h"
#include "PhysicsManager.h"
#include "Transform.h"
#include "GameObject.h"
namespace GlassEngine
{
	PointGravity::PointGravity()
	{

	}
	
	PointGravity::~PointGravity()
	{

	}

	void PointGravity::Start()
	{

	}

	void PointGravity::Update()
	{
	}

	void PointGravity::FixedUpdate()
	{
		//FINISH ME
		GetRigidbodies();
		for (auto r : rigidbodies)
		{
			Vec3d colPos = r->GetParent()->GetTransform()->GetPosition();
			Vec3d gravityCenter = this->GetParent()->GetTransform()->GetPosition();
			Vec2d colPos_(colPos.x, colPos.y);
			Vec2d gravPos_ = Vec2d(gravityCenter.x, gravityCenter.y);
			auto d = Distance(gravPos_, colPos_);
			Vec2d p = gravPos_ - colPos_;
			auto n = p.Normalize();
			Vec2d result = Vec2d(n.x, n.y) * (1.0f - d / (range)) * gravityStrength;
			r->AddForce(result);
		}
			//r->AddForce()
	}

	void PointGravity::Stop()
	{

	}

	void PointGravity::Init(float range_, float strength_, Vec2d offset_)
	{
		range = range_;
		gravityStrength = strength_;
		offset = offset_;
	}

	void PointGravity::GetRigidbodies()
	{
		/*for (auto r : rigidbodies)
		{ 
			auto d = Distance(r->GetParent()->GetTransform()->GetPosition(), this->parentObject->GetTransform()->GetPosition());
			if (d > range)
				r->ResetVelocity();
		}*/

		rigidbodies.clear();
		for (auto r : Physics.GetRigidbodies())
		{
			if (Distance(r->GetParent()->GetTransform()->GetPosition(), this->parentObject->GetTransform()->GetPosition()) < range)
			{
				if (r->GetParent()->GetComponent(RigidbodyC))
					rigidbodies.push_back(std::dynamic_pointer_cast<Rigidbody>(r->GetParent()->GetComponent(RigidbodyC)));
			}
		}
	}
}