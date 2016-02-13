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

	void SpriteCollider::GenerateMask(BYTE* imageData, const Rect& rect)
	{
		// Sanity check
		assert(sizeof(DWORD) == 4);

		// How many DWORDs across do we need? 
		int numDwordsAcross = (rect.Width() + 31) / 32;

		// allocate	memory for the collision mask and clear
		DWORD *mask_ = new DWORD[numDwordsAcross * rect.Height()];
		memset(mask_, 0, numDwordsAcross * rect.Height() * 4);

		// Set up start pointers
		BYTE *sourcePntr = imageData;
		DWORD *maskPointer = mask_;
		DWORD currentBit = 0;

		for (int y = 0; y < rect.Height(); y++)
		{
			for (int x = 0; x < rect.Width(); x++)
			{
				BYTE alpha = sourcePntr[3];

				// Each pixel can be represented as one bit, either 1 for solid or 0 for empty
				// Here 1 is when alpha is 255 only but I guess you could say if the alpha is > some amount it is counted as solid - up to the game
				if (alpha == 255)
				{
					*maskPointer = *maskPointer | (1 << currentBit); // turn on the correct bit in the current mask DWORD
				}

				sourcePntr += 4;
				currentBit++;
				if (currentBit == 32)
				{
					// When we reach the last bit we move on to the next DWORD and reset bit to 0
					currentBit = 0;
					maskPointer++;
				}
			}
		}
		mask = mask_;
		boundingBox = rect;
		dwordSize = numDwordsAcross;
	}

	bool SpriteCollider::CheckCollider(SmartPtr<SpriteCollider> collider)
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
				BYTE alpha = (BYTE)mask[offset];

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