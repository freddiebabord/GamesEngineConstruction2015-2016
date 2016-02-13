#if !defined (SPRITECOLLIDER)
#define SPRITECOLLIDER

#include "Collider.h"

namespace GlassEngine{

	class Sprite;
	class SpriteSheet;

	class SpriteCollider : public Collider
	{
	public:
		SpriteCollider(SmartPtr<GameObject> base) : Collider(base) { id = ColliderC; };
		SpriteCollider() : Collider() { id = ColliderC; };
		virtual ~SpriteCollider();

		void SpriteCollider::GenerateMask(BYTE* imageData, const Rect& rect);
		bool CheckCollider(SmartPtr<SpriteCollider> collider);


		DWORD* GetMask() const { return mask; };
		Vec2i GetMaskSize() const { return maskSize; };
		Rect getBoundingBox() const { return boundingBox; };
		int getDwordSize() const { return dwordSize; };

	private:
		Rect boundingBox;
		DWORD* mask = nullptr;
		Vec2i maskSize = Vec2i(0);
		int dwordSize = 0;
	};

}

#endif //SPRITECOLLIDER