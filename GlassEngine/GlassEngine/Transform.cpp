#include "precomp.h"
#include "Transform.h"
#include "Transform.h"
#include "GameObject.h"

namespace GlassEngine
{

	void Transform::Translate(Vec3i dir, double amount)
	{
		Vec3i magnitde = dir * (int)amount;
		SetPosition(position + magnitde);
	}

	void Transform::Translate(Vec3i translationAmount)
	{
		SetPosition(position + translationAmount);
	}

};