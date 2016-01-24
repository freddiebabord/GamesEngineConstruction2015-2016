#if !defined (TRANSFORM)
#define TRANSFORM

#include "precomp.h"
#include "Component.h"

namespace GlassEngine{

	class GameObject;

	class Transform : public Component
	{
	public:
		Transform(std::shared_ptr<GameObject> parentObj_) : Component(parentObj_){ id = TransformC; position = Vec3d(0); };
		Transform() : Component(){};
		~Transform(){};

		void Start() override {};
		void Update()override {};
		void Stop()override {};

		const Vec3d GetPosition(){ return position; };
		const Vec3d GetRotation(){ return rotation; };
		const Vec3d GetScale(){ return scale; };

		void SetPosition(const Vec3d newPos){ position = newPos; };
		void SetRotation(const Vec3d newRot){ rotation = newRot; };
		void SetScale(const Vec3d newScale){ scale = newScale; };

		void Translate(Vec3d dir, double amount)
		{
			Vec3d magnitde = dir * (int)amount;
			SetPosition(position + magnitde);
		};

		void Translate(Vec3d translationAmount)
		{
			SetPosition(position + translationAmount);
		}

	private:
		Vec3d position = Vec3d(0.0);
		Vec3d rotation = Vec3d(0.0);
		Vec3d scale = Vec3d(0.0);
	};

}

#endif