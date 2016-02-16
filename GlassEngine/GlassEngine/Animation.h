#if !defined (ANIMATION)
#define ANIMATION

#include "Component.h"

namespace GlassEngine{

	class SpriteSheet;

	struct AnimationClip
	{
		AnimationClip(std::string name, Vec2i newBoundary)
		{
			boundaries = newBoundary;
			animationName = name;
		};
		AnimationClip(Vec2i newBoundary)
		{
			boundaries = newBoundary;
		};
		std::string animationName = "";
		Vec2i boundaries;
	};

	class Animation : public Component
	{
	public:
		Animation(SmartPtr<GameObject> parentObj) : Component(parentObj){ id = AnimationC; };
		virtual ~Animation();

		void Update() override final;

		bool AddAnimation(AnimationClip newAnim);
		void SetAnimSpeed(const int newAnimSpeed){ animationSpeed = newAnimSpeed; };
		void SetCurrentAnimation(const std::string& newAnimRef, bool randomFrame = false);
		
		Vec2i GetAnimations(const std::string& animationIndex)  { return animations[animationIndex]; };
		int GetCurrentSprite()const{ return currentSprite; };

		void Loop(const bool shouldLoop){ looping = shouldLoop; };

		void SetProperties(bool randomFrame = false);
	protected:

	private:
		void Animate();

		std::string currentAnimation = "";
		int currentSprite = 0;
		int animationSpeed = 5;
		double animationTime = 0;
		std::map<std::string, Vec2i> animations;

		bool looping = true;

		int currentAnimationStart = 0;
		int currentAnimationEnd = 0;
	};
}

#endif //ANIMATION