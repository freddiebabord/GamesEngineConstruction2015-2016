#include "precomp.h"
#include "SpriteSheet.h"

namespace GlassEngine{


	SpriteSheet::~SpriteSheet()
	{
		animations.clear();
	}

	const Vec2i SpriteSheet::GetIdvSpriteDims()
	{
		return individualSpriteSize;
	}

	bool SpriteSheet::AddAnimation(Animation newAnim)
	{
		animations.push_back(newAnim);
		return true;
	}

	void SpriteSheet::Update()
	{
		Animate();
	}


	void SpriteSheet::Animate()
	{
		if (currentSprite < animations[currentAnimation].boundaries.start)
			currentSprite = animations[currentAnimation].boundaries.start;
		if (currentSprite >= animations[currentAnimation].boundaries.end)
			currentSprite = animations[currentAnimation].boundaries.start;

		if (animationTime < HAPI->GetTime() - (1000 / animationSpeed))
		{
			animationTime = (float)HAPI->GetTime();
			currentSprite++;
		}
	}
}