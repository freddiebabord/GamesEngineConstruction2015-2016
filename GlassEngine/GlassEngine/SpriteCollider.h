#if !defined (SPRITECOLLIDER)
#define SPRITECOLLIDER

#include "Collider.h"

namespace GlassEngine{

	class Sprite;
	class SpriteSheet;

	class SpriteCollider : public Collider
	{
	public:
		SpriteCollider(std::shared_ptr<GameObject> parentObj_) : Collider(parentObj_) { id = ColliderC; };
		~SpriteCollider();

		void GenerateMask(std::shared_ptr<Sprite> sprite);
		void GenerateMask(std::shared_ptr<SpriteSheet> spriteSheet);
		bool CheckCollider(std::shared_ptr<SpriteCollider> collider);
		BYTE* GetMask() const { return mask; };
		Vec2i GetMaskSize() const { return maskSize; };

	private:
		BYTE* mask = nullptr;
		Vec2i maskSize = Vec2i(0);
	};

}

#endif //SPRITECOLLIDER