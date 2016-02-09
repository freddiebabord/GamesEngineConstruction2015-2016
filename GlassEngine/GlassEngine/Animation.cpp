#include "precomp.h"
#include "Animation.h"
#include "SpriteSheet.h"

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
		Animate();
	}

	void Animation::Animate()
	{
		if (currentSprite < animations[currentAnimation].start)
			currentSprite = animations[currentAnimation].start;
		if (currentSprite >= animations[currentAnimation].end)
			currentSprite = animations[currentAnimation].start;

		if (animationTime < HAPI->GetTime() - (1000 / animationSpeed))
		{
			animationTime = (float)HAPI->GetTime();
			currentSprite++;
		}
	}

}