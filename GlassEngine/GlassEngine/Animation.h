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
		void SetCurrentAnimation(const std::string& newAnimRef){ currentAnimation = newAnimRef; };
		
		Vec2i GetAnimations(const std::string& animationIndex)  { return animations[animationIndex]; };
		int GetCurrentSprite()const{ return currentSprite; };


	protected:

	private:
		void Animate();
		
		std::string currentAnimation = "";
		int currentSprite = 0;
		int animationSpeed = 5;
		float animationTime = 0.f;
		std::map<std::string, Vec2i> animations;
	};
}

#endif //ANIMATION