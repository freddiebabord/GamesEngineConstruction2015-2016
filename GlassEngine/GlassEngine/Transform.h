#if !defined (TRANSFORM)
#define TRANSFORM

#include "precomp.h"
#include "Component.h"

namespace GlassEngine{

	class GameObject;

	class Transform : public Component
	{
	public:
		Transform(std::shared_ptr<GameObject> parentObj_) : Component(parentObj_){ id = TransformC; position = Vec3i(0); };
		Transform() : Component(){};
		~Transform(){};

		void Start() override {};
		void Update()override {};
		void Stop()override {};

		const Vec3i GetPosition(){ return position; };
		const Vec3d GetRotation(){ return rotation; };
		const Vec3d GetScale(){ return scale; };

		void SetPosition(const Vec3i newPos){ position = newPos; };
		void SetRotation(const Vec3d newRot){ rotation = newRot; };
		void SetScale(const Vec3d newScale){ scale = newScale; };

		void Translate(Vec3i dir, double amount);
		void Translate(Vec3i translationAmount);

	private:
		Vec3i position{ 0, 0, 0 };
		Vec3d rotation{ 0.0,0.0,0.0 };
		Vec3d scale {1.0,1.0,1.0};
	};

}

#endif