#include "precomp.h"
#include "SpriteCollider.h"
#include "Sprite.h"
#include "SpriteSheet.h"

namespace GlassEngine{

	SpriteCollider::~SpriteCollider()
	{
		if (mask != nullptr)
			delete[] mask;
	}

	void SpriteCollider::GenerateMask(std::shared_ptr<Sprite> sprite)
	{
		Vec2i spriteDims = sprite->GetSpriteDims();

		//A local "rectangle" which handles image clipping
		int startX = 0;
		int startY = 0;
		int endX = spriteDims.width;
		int endY = spriteDims.height;

		BYTE noColl[4] = { 0, 0, 0, 0 };
		BYTE Coll[4] = { 1, 1, 1, 1};

		BYTE* imgPtr = sprite->GetImage();
		mask = new BYTE[endX * endY * 4];

		for (int y = startY; y < endY; ++y)
		{
			for (int x = startX; x < endX; ++x)
			{

				int offset = ((y * spriteDims.width) + x) * 4;


				BYTE alpha = imgPtr[offset + 3];


				if (alpha <= 0)
				{
					memcpy(&mask[offset], &noColl, 4);
				}
				else
				{
					memcpy(&mask[offset], &Coll, 4);
				}
			}
		}

		maskSize = spriteDims;
	}

	void SpriteCollider::GenerateMask(std::shared_ptr<SpriteSheet> spriteSheet)
	{
		Vec2i spriteDims = spriteSheet->GetIdvSpriteDims();

		//A local "rectangle" which handles image clipping
		int startX = 0;
		int startY = 0;
		int endX = spriteDims.width;
		int endY = spriteDims.height;

		BYTE noColl[4] = { 0, 0, 0, 0 };
		BYTE Coll[4] = { 1, 1, 1, 1 };

		BYTE* imgPtr = spriteSheet->GetImage();
		mask = new BYTE[endX * endY * 4];

		for (int y = startY; y < endY; ++y)
		{
			for (int x = startX; x < endX; ++x)
			{

				int offset = ((y * spriteDims.width) + x) * 4;


				BYTE alpha = imgPtr[offset + 3];


				if (alpha <= 0)
				{
					memcpy(&mask[offset], &noColl, 4);
				}
				else
				{
					memcpy(&mask[offset], &Coll, 4);
				}
			}
		}

		maskSize = spriteDims;
	}

	bool SpriteCollider::CheckCollider(std::shared_ptr<SpriteCollider> collider)
	{
		Vec2i otherMaskSize = collider->GetMaskSize();

		int startX = 0;
		int startY = 0;
		int endX = maskSize.width;
		int endY = maskSize.height;

		for (int y = startY; y < endY; ++y)
		{
			for (int x = startX; x < endX; ++x)
			{
				//Offset of the current pixel for 4 BYTES (RGBA)
				int offset = ((y * maskSize.width) + x) * 4;
				int otherMaskOffset = ((y * otherMaskSize.width) + x) * 4;
				//Gets the alpha value from the images data by the offset for the alpha BYTE of the pixel
				BYTE alpha = mask[offset];

				//Coninue the next loop
				if (alpha == 0 && collider->GetMask()[otherMaskOffset] == 0)
					continue;
				else
				{
					return true;
				}
			}
		}
		return false;
	}
}