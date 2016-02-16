#include "precomp.h"
#include "Animation.h"
#include "SpriteSheet.h"
#include "Time.h"
#include "GameObject.h"

namespace GlassEngine{

	Animation::~Animation(){
		animations.clear();
	}

	bool Animation::AddAnimation(AnimationClip newAnim)
	{
		animations[newAnim.animationName] = newAnim.boundaries;
		return true;
	}

	void Animation::Update()
	{
		if (enabled)
			Animate();
	}

	void Animation::SetCurrentAnimation(const std::string& newAnimRef, bool randomFrame)
	{
		currentAnimation = newAnimRef; 
		SetProperties(randomFrame);
	}

	void Animation::SetProperties(bool randomFrame)
	{
		currentAnimationStart = animations[currentAnimation].start;
		currentAnimationEnd = animations[currentAnimation].end;
		if (randomFrame)
			currentSprite = rand() % currentAnimationEnd - currentAnimationEnd;
	}

	void Animation::Animate()
	{
		if (currentSprite < currentAnimationStart)
			currentSprite = currentAnimationStart;
		if (currentSprite >= currentAnimationEnd)
		{
			if (looping)
				currentSprite = currentAnimationStart;
			else
			{
				parentObject->isActive(false);
				currentSprite = currentAnimationStart;
				return;
			}
		}

		if (animationTime < HAPI->GetTime() - (1000 / animationSpeed))
		{
			animationTime = (float)HAPI->GetTime();
			currentSprite++;
		}
	}

}