#if !defined (POINTGRAVITY)
#define POINTGRAVITY

#include "Component.h"
#include "Rigidbody.h"

namespace GlassEngine{

	class PointGravity : public Component
	{
	public:
		PointGravity(std::shared_ptr<GameObject> parentObj_) : Component(parentObj_){ id = PointGravityC; };
		PointGravity();
		~PointGravity();

		void Start() override final;
		void Update() override final;
		void FixedUpdate() override final;
		void Stop() override final;

		void Init(float range_, float strength_, Vec2d offset_);

	private:
		void GetRigidbodies();

		std::vector<std::shared_ptr<Rigidbody>> rigidbodies;
		float range = 5.0f;
		float gravityStrength = 5.0f;
		Vec2d offset = Vec2d(0.0);
	};

}
#endif