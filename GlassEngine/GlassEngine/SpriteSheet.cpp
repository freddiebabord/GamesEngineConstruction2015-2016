#include "precomp.h"
#include "SpriteSheet.h"

namespace GlassEngine{

	SpriteSheet::~SpriteSheet()
	{
		if (imageData != nullptr)
			delete[] imageData;
	}

	const Vec2i SpriteSheet::GetIdvSpriteDims()
	{
		return individualSpriteSize;
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