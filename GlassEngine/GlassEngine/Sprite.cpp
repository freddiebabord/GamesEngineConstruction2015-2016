#include "precomp.h"
#include "Sprite.h"

namespace GlassEngine{

	Sprite::~Sprite()
	{
		if (imageData != nullptr)
			delete[] imageData;
	}

	void Sprite::Stop()
	{
		if (imageData != nullptr)
			delete[] imageData;
	}

	BYTE* Sprite::GetImage()
	{
		return imageData;
	}

	bool Sprite::LoadSprite(std::string path)
	{
		if (!HAPI->LoadTexture(path, &imageData, &spriteDims.width, &spriteDims.height))
		{
			HAPI->UserMessage("Error! Unable to load sprite!" + path, "Glass Engine");
			return false;
		}
		return true;
	}

	Vec2i Sprite::GetSpriteDims()
	{
		return spriteDims;
	}
}