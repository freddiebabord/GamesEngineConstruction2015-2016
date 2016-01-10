#if !defined (SPRITESHEET)
#define SPRITESHEET

#include "Component.h"

namespace GlassEngine{

	struct Animation
	{
		Animation(Vec2i newBoundary)
		{
			boundaries = newBoundary;
		};

		Vec2i boundaries;
	};

	//FINISH ME
	class SpriteSheet : public Component
	{
	public:
		SpriteSheet(std::shared_ptr<GameObject> parentObj) :Component(parentObj){ id = SpriteSheetC; };
		SpriteSheet(std::shared_ptr<GameObject> parentObj, const std::string& path) :Component(parentObj){ id = SpriteSheetC;  LoadSprite(path); };
		SpriteSheet(std::shared_ptr<GameObject> parentObj, const std::string& path, Vec2i idvSpriteSize) : Component(parentObj){
			id = SpriteSheetC;
			LoadSprite(path);
			individualSpriteSize = idvSpriteSize;
		};
		SpriteSheet(const std::string& path, Vec2i idvSpriteSize) : Component(path){
			LoadSprite(path);
			individualSpriteSize = idvSpriteSize;
		};
		virtual ~SpriteSheet();

		void Update() override final;

		const int GetRowCount(){ return rowCount; };
		const Vec2i GetIdvSpriteDims();
		void SetIdvSpriteSize(const Vec2i size){ individualSpriteSize = size; };
		Animation GetAnimations(const int& animationIndex) { return animations[animationIndex]; };
		bool AddAnimation(Animation newAnim);

		void SetCurrentAnimation(const int newAnimRef){ currentAnimation = newAnimRef; };
		const int GetCurrentSprite(){ return currentSprite; };

	protected:
		void Animate();

	private:
		Vec2i individualSpriteSize = Vec2i(0);
		int currentSprite = 0;
		int rowCount = 1;
		std::vector<Animation> animations;
		int currentAnimation = 0;
		float animationTime = 0.f;
		int animationSpeed = 5;
	};

}

#endif //SPRITESHEET