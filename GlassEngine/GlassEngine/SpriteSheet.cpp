#include "precomp.h"
#include "SpriteSheet.h"

namespace GlassEngine{


	SpriteSheet::~SpriteSheet()
	{
		if (imageData != nullptr)
			delete[] imageData;
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

	BYTE* SpriteSheet::GetImage()
	{
		return imageData;
	}

	bool SpriteSheet::LoadSprite(std::string path)
	{
		if (!HAPI->LoadTexture(path, &imageData, &spriteDims.width, &spriteDims.height))
		{
			HAPI->UserMessage("Error! Unable to load sprite!" + path, "Glass Engine");
			return false;
		}
		return true;
	}

	Vec2i SpriteSheet::GetSpriteDims()
	{
		return spriteDims;
	}
}