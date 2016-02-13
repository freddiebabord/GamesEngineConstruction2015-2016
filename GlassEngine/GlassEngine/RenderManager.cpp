#include "precomp.h"
#include "RenderManager.h"
#include "Sprite.h"
#include "SpriteSheet.h"
#include "Transform.h"
#include "GameObject.h"
#include "Animation.h"

namespace GlassEngine{

	RenderManager *RenderManager::instance{ nullptr };

	// Creates / get a singleton instance of the rendering manager
	RenderManager &RenderManager::Instance()
	{
		if (instance == nullptr)
			instance = new RenderManager;
		return *instance;
	}

	//Starts the rendering manager with the supplied screens dimentions and whether its fullscreen or not
	void RenderManager::Start(int Width, int Height, bool fullscreen)
	{
		int screenWidth = Width, screenHeight = Height;

		if (fullscreen)
		{
			//Windows specific functions to wind the screen resolution of the primary monitor
			RECT desktop;
			HWND hDesktop = GetDesktopWindow();
			GetWindowRect(hDesktop, &desktop);
			//Sets windows specific icons
			SendMessage(hDesktop, WM_SETICON, ICON_SMALL, (LPARAM)IDI_ICON1);
			SendMessage(hDesktop, WM_SETICON, ICON_BIG, (LPARAM)IDI_ICON1);
			//Sets the local screen variables to initialise the window
			screenWidth = desktop.right;
			screenHeight = desktop.bottom;
		}
		else
		{
			//Set a fixed default screen size to initialise the window
			screenHeight = Height;
			screenWidth = Width;
		}

		//Start the HAPI API to create the window
#if defined(_DEBUG)
		if (!HAPI->Initialise(&screenWidth, &screenHeight))
		{
			return;
		}
#else
		if (!HAPI->Initialise(&screenWidth, &screenHeight, HAPI_INIT_FULLSCREEN))
		{
			return;
		}
#endif
		//Create new unique screen data pointer
		screen = std::make_unique<Screen>(screenWidth, screenHeight, HAPI->GetScreenPointer());

		dirtyRectangles.reserve(sizeof(DirtyRectangle) * 50);
	}

	void RenderManager::Update()
	{
		for (const auto r : dirtyRectangles)
			RenderDR(r.position, r.size);
		dirtyRectangles.clear();

		for (auto s : sprites)
			s->Update();

		for (auto s : spritesheets)
			s->Update();
	}

	// Clear the screen to a blanket colour by copying all of the pixels that were already set and then pasting them into the next set of the array. Fills the screen exponentially.
	void RenderManager::ClearScreen(const HAPI_TColour& backgroundColour)
	{
		//Set the first pixel to be that of the background colour
		memmove(screen->screenData, &backgroundColour, 4);

		int currentPixel = 0;

		//Copy the pixels in powers of 2 so it fills more than half of the screen
		for (int i = 1; i * 2 < screen->screenDimentions.width*screen->screenDimentions.height; i *= 2)
		{
			memmove(screen->screenData + i * 4, screen->screenData, i * 4);
			currentPixel = i;
		}

		//Find the remaining amount of pixels
		int remainingPixels = (screen->screenDimentions.width*screen->screenDimentions.height) - currentPixel;

		//copy half of the screen to the latter half - this will overwrite some existing pixels
		for (int i = (screen->screenDimentions.width * screen->screenDimentions.height) / 2; i < screen->screenDimentions.width*screen->screenDimentions.height; i *= 2)
		{
			memmove(screen->screenData + i * 4, screen->screenData, i * 4);
			currentPixel = i;
		}
	};

	//Render an image on screen
	//int spriteIndex: The index of the sprite in the render managers vector of sprites
	//Vec3 renderPos: The position in 3D space the image should be rendered at
	void RenderManager::RenderBackground(const int& spriteIndex, const Vec3d& renderPos)
	{
		Vec2i screenDims = screen->screenDimentions;
		Vec2i spriteDims = sprites[spriteIndex]->GetSpriteDims();

		Vec2d finalRenderPos = Vec2d(renderPos.x, renderPos.y);
		finalRenderPos.x -= spriteDims.x / 2;
		finalRenderPos.y -= spriteDims.y / 2;

		//If the image is off of the screen dont execute the render function
		if (renderPos.x >= screenDims.width || renderPos.y >= screenDims.height || renderPos.x + spriteDims.width < 0 || renderPos.y + spriteDims.height < 0)
			return;

		//A local "rectangle" which handles image clipping
		int startX = 0;
		int startY = 0;
		int endX = 0;
		int endY = 0;


		BYTE* imgPtr = sprites[spriteIndex]->GetImage();
		BYTE* screenPtr = screen->screenData;

		//If the image is off of the left side of the screen offset the left side of the rectangle with the amount its off of the screen by
		if (renderPos.x < 0)
			startX = abs((int)renderPos.x);
		//If the image is off of the top side of the screen offset the top side of the rectangle with the amount its off of the screen by
		if (renderPos.y < 0)
			startY = abs((int)renderPos.y);
		//If the image is off of the right side of the screen offset the right side of the rectangle with the amount its off of the screen by
		if (renderPos.x + spriteDims.width > screenDims.width)
			endX = (spriteDims.width + (int)renderPos.x) - screenDims.width;
		//If the image is off of the bottom side of the screen offset the bottom side of the rectangle with the amount its off of the screen by
		if (renderPos.y + spriteDims.height > screenDims.height)
			endY = ((int)renderPos.y + spriteDims.height) - screenDims.height;

		//The rendering loop for each pixal of the images clipped rectangle
		for (int y = startY; y < spriteDims.height - endY; ++y)
		{
			for (int x = startX; x < spriteDims.width - endX; ++x)
			{
				//Offset of the current pixel for 4 BYTES (RGBA)
				int offset = ((y * spriteDims.width) + x) * 4;

				//Gets the alpha value from the images data by the offset for the alpha BYTE of the pixel
				BYTE alpha = imgPtr[offset + 3];

				//Coninue the next loop
				if (alpha <= 0)
					continue;

				//Sets a screen offset for where the BYTE data needs to be written to the screen
				int scrOffset = ((((int)renderPos.y + y) * screenDims.width) + (((int)renderPos.x + x))) * 4;

				//If the image doesn't have transparency copy the data from the sprites data to the screen for (RGB)
				//Note the 4th alpha BYTE of the screen is being used as a z-index
				if (alpha >= 255)
				{
					memmove(&screenPtr[scrOffset], &imgPtr[offset], 3);
				}
				else
				{
					//Otherwise get each BYTE of colour from the image
					BYTE blue = imgPtr[offset];
					BYTE red = imgPtr[offset + 1];
					BYTE green = imgPtr[offset + 2];

					//And lerp the screens BYTE data with the sprites BYTE data using the amount of alpha as a percentage offset
					screenPtr[(int)scrOffset] = screenPtr[(int)scrOffset] + ((alpha * (blue - screenPtr[(int)scrOffset])) >> 8);
					screenPtr[(int)scrOffset + 1] = screenPtr[(int)scrOffset + 1] + ((alpha*(red - screenPtr[(int)scrOffset + 1])) >> 8);
					screenPtr[(int)scrOffset + 2] = screenPtr[(int)scrOffset + 2] + ((alpha*(green - screenPtr[(int)scrOffset + 2])) >> 8);
				}
			}
		}

	};

	//Render an image on screen
	//int spriteIndex: The index of the sprite in the render managers vector of sprites
	//Vec3 renderPos: The position in 3D space the image should be rendered at
	void RenderManager::Render(const int& spriteIndex, const Vec3d& renderPos)
	{
		Vec2i screenDims = screen->screenDimentions;
		Vec2i spriteDims = sprites[spriteIndex]->GetSpriteDims();

		Vec2d finalRenderPos = Vec2d(renderPos.x, renderPos.y);
		finalRenderPos.x -= spriteDims.x / 2;
		finalRenderPos.y -= spriteDims.y / 2;

		//If the image is off of the screen dont execute the render function
		if (finalRenderPos.x >= screenDims.width || finalRenderPos.y >= screenDims.height || finalRenderPos.x + spriteDims.width < 0 || finalRenderPos.y + spriteDims.height < 0)
			return;

		//A local "rectangle" which handles image clipping
		int startX = 0;
		int startY = 0;
		int endX = 0;
		int endY = 0;

		
		BYTE* imgPtr = sprites[spriteIndex]->GetImage();
		BYTE* screenPtr = screen->screenData;

		//If the image is off of the left side of the screen offset the left side of the rectangle with the amount its off of the screen by
		if (finalRenderPos.x < 0)
			startX = abs((int)finalRenderPos.x);
		//If the image is off of the top side of the screen offset the top side of the rectangle with the amount its off of the screen by
		if (finalRenderPos.y < 0)
			startY = abs((int)finalRenderPos.y);
		//If the image is off of the right side of the screen offset the right side of the rectangle with the amount its off of the screen by
		if (finalRenderPos.x + spriteDims.width > screenDims.width)
			endX = (spriteDims.width + (int)finalRenderPos.x) - screenDims.width;
		//If the image is off of the bottom side of the screen offset the bottom side of the rectangle with the amount its off of the screen by
		if (finalRenderPos.y + spriteDims.height > screenDims.height)
			endY = ((int)finalRenderPos.y + spriteDims.height) - screenDims.height;

		//The rendering loop for each pixal of the images clipped rectangle
		for (int y = startY; y < spriteDims.height - endY; ++y)
		{
			for (int x = startX; x < spriteDims.width - endX; ++x)
			{
				//Offset of the current pixel for 4 BYTES (RGBA)
				int offset = ((y * spriteDims.width) + x) * 4;
				
				//Gets the alpha value from the images data by the offset for the alpha BYTE of the pixel
				BYTE alpha = imgPtr[offset + 3];

				//Coninue the next loop
				if (alpha <= 0)
					continue;

				//Sets a screen offset for where the BYTE data needs to be written to the screen
				int scrOffset = ((((int)finalRenderPos.y + y) * screenDims.width) + (((int)finalRenderPos.x + x))) * 4;

				//If the image doesn't have transparency copy the data from the sprites data to the screen for (RGB)
				//Note the 4th alpha BYTE of the screen is being used as a z-index
				if (alpha >= 255)
				{
					memmove(&screenPtr[scrOffset], &imgPtr[offset], 3);
				}
				else
				{
					//Otherwise get each BYTE of colour from the image
					BYTE blue = imgPtr[offset];
					BYTE red = imgPtr[offset + 1];
					BYTE green = imgPtr[offset + 2];

					//And lerp the screens BYTE data with the sprites BYTE data using the amount of alpha as a percentage offset
					screenPtr[(int)scrOffset] = screenPtr[(int)scrOffset] + ((alpha * (blue - screenPtr[(int)scrOffset])) >> 8);
					screenPtr[(int)scrOffset + 1] = screenPtr[(int)scrOffset + 1] + ((alpha*(red - screenPtr[(int)scrOffset + 1])) >> 8);
					screenPtr[(int)scrOffset + 2] = screenPtr[(int)scrOffset + 2] + ((alpha*(green - screenPtr[(int)scrOffset + 2])) >> 8);
				}
			}
		}

		AddDirtyRectangle(DirtyRectangle(Vec2i((int)renderPos.x, (int)renderPos.y), spriteDims));


	};


	//Render an image on screen
	//int spriteIndex: The index of the spritesheet in the render managers vector of spritesheet 
	//int idvSpriteIndex: The index of the sprite frame for the spritesheet 
	//Vec3 renderPos: The position in 3D space the image should be rendered at
	void RenderManager::Render(const int& spriteIndex, const Vec3d& renderPos, const int& idvSpriteIndex)
	{
		//The current spriesheet from the index
		SmartPtr<SpriteSheet> sheet = spritesheets[spriteIndex];

		Vec2i screenDims = screen->screenDimentions;
		Vec2i spriteIdvDims = sheet->GetIdvSpriteDims();
		Vec2i spriteDims = sheet->GetSpriteDims();

		Vec2d finalRenderPos = Vec2d(renderPos.x, renderPos.y);
		finalRenderPos.x -= spriteIdvDims.x / 2;
		finalRenderPos.y -= spriteIdvDims.y / 2;

		//The local X, Y on the sprite sheet
		int spriteY = idvSpriteIndex / (spriteDims.width / spriteIdvDims.width);
		int spriteX = idvSpriteIndex % (spriteDims.width / spriteIdvDims.width);

		//if the sprite requeststed is off of the sheet dont contiue the function
		if (spriteX >= spriteDims.width / spriteIdvDims.width ||
			spriteY >= spriteDims.height / spriteIdvDims.height)
			return;

		//if the rendered position is off of the screen dont render
		if (finalRenderPos.x >= screenDims.width ||
			finalRenderPos.y >= screenDims.height ||
			finalRenderPos.x + spriteIdvDims.width < 0 ||
			finalRenderPos.y + spriteIdvDims.height < 0)
			return;

		//A local "rectangle" which handles image clipping
		int startX = 0;
		int startY = 0;
		int endX = 0;
		int endY = 0;

		//If the image is off of the left side of the screen offset the left side of the rectangle with the amount its off of the screen by
		if (finalRenderPos.x < 0)
			startX = abs((int)finalRenderPos.x); 
		//If the image is off of the top side of the screen offset the top side of the rectangle with the amount its off of the screen by
		if (finalRenderPos.y < 0)
			startY = abs((int)finalRenderPos.y);
		//If the image is off of the right side of the screen offset the right side of the rectangle with the amount its off of the screen by
		if (finalRenderPos.x + spriteIdvDims.width > screenDims.width)
			endX = (spriteIdvDims.width + (int)finalRenderPos.x) - screenDims.width;
		//If the image is off of the bottom side of the screen offset the bottom side of the rectangle with the amount its off of the screen by
		if (finalRenderPos.y + spriteIdvDims.height > screenDims.height)
			endY = ((int)finalRenderPos.y + spriteIdvDims.height) - screenDims.height;

		//The local offset for the sprite in the spritesheet
		int spriteOffset = ((spriteY * spriteDims.width * spriteIdvDims.height) + (spriteX * spriteIdvDims.width)) * 4;

		BYTE* imgPtr = sheet->GetImage();
		BYTE* screenPtr = screen->screenData;

		int yLoopEnd = spriteIdvDims.height - endY;
		int xLoopEnd = spriteIdvDims.width - endX;

		//The rendering loop for each pixal of the images clipped rectangle
		for (int y = startY; y < yLoopEnd; ++y)
		{
			for (int x = startX; x < xLoopEnd; ++x)
			{
				//Offset of the current pixel for 4 BYTES (RGBA)
				int offset = ((y * spriteDims.width) + x) * 4;
				
				//Gets the alpha value from the images data by the offset for the alpha BYTE of the pixel
				BYTE alpha = imgPtr[offset + spriteOffset + 3];
				
				//Coninue the next loop
				if (alpha <= 0) continue;

				int scrOffset = (((int)(finalRenderPos.y + y)  * screenDims.width) + ((int)(finalRenderPos.x + x))) * 4;

				//If the image doesn't have transparency copy the data from the sprites data to the screen for (RGB)
				//Note the 4th alpha BYTE of the screen is being used as a z-index
				if (alpha >= 255)
				{
					memmove(&screenPtr[scrOffset], &imgPtr[offset + spriteOffset], 3);
				}
				else
				{
					//Otherwise get each BYTE of colour from the image
					BYTE blue = imgPtr[offset + spriteOffset];
					BYTE red = imgPtr[offset + spriteOffset + 1];
					BYTE green = imgPtr[offset + spriteOffset + 2];

					//And lerp the screens BYTE data with the sprites BYTE data using the amount of alpha as a percentage offset
					screenPtr[(int)scrOffset] = screenPtr[(int)scrOffset] + ((alpha * (blue - screenPtr[(int)scrOffset])) >> 8);
					screenPtr[(int)scrOffset + 1] = screenPtr[(int)scrOffset + 1] + ((alpha*(red - screenPtr[(int)scrOffset + 1])) >> 8);
					screenPtr[(int)scrOffset + 2] = screenPtr[(int)scrOffset + 2] + ((alpha*(green - screenPtr[(int)scrOffset + 2])) >> 8);
				}
			}
		}

		AddDirtyRectangle(DirtyRectangle(Vec2i((int)renderPos.x, (int)renderPos.y), spriteIdvDims));

	};


	//Returns a Vector2<int> of the screen dimentions
	const Vec2i RenderManager::GetScreenDimentions()
	{
		return screen->screenDimentions;
	}

	// Add a sprite to the render manager by loading in a sprite from a specified path string
	bool RenderManager::AddSprite(const std::string& path)
	{
		sprites.push_back(MakeSmartPtr<Sprite>(path));
		return true;
	}

	// Add a sprite to the render manager by a pointer to a block of memory which contains the sprite
	bool RenderManager::AddSprite(SmartPtr<Sprite> spriteToAdd)
	{
		sprites.push_back(spriteToAdd);
		return true;
	}

	// Add a spritesheet to the render manager by loading in a sprite from a specified path string
	bool RenderManager::AddSpriteSheet(const std::string& path, Vec2i spriteSize)
	{
		spritesheets.push_back(MakeSmartPtr<SpriteSheet>(path, spriteSize));
		return true;
	}

	// Add a spritesheet to the render manager by a pointer to a block of memory which contains the sprite
	bool RenderManager::AddSpriteSheet(SmartPtr<SpriteSheet> spriteToAdd)
	{
		spritesheets.push_back(spriteToAdd);
		return true;
	}

	//Dirty rectangles for an image
	//Vector 2 pos: the top left position that has to be cleared from
	//Vector 2 size: the amount that needs to be cleared from the positon point
	void RenderManager::RenderDR(const Vec2i& pos, const Vec2i& size)
	{
		Vec2i screenDims = screen->screenDimentions;
		Vec2d finalRenderPos = Vec2d(pos.x, pos.y);
		finalRenderPos.x -= size.x / 2;
		finalRenderPos.y -= size.y / 2;
		//If the position is off the screen then dont dirty rect
		if (finalRenderPos.x >= screenDims.width || finalRenderPos.y >= screenDims.height || finalRenderPos.x + size.x < 0 || finalRenderPos.y + size.y < 0)
			return;

		//A local "rectangle" which handles image clipping
		int startX = 0;
		int startY = (int)finalRenderPos.y;
		int endX = (int)size.x * 4;
		int endY = (int)finalRenderPos.y + (int)size.y;

		//If the rect is off of the left side of the screen offset the left side of the rectangle with the amount its off of the screen by
		if (finalRenderPos.x < 0)
			startX = abs((int)finalRenderPos.x);
		//If the rect is off of the top side of the screen offset the top side of the rectangle with the amount its off of the screen by
		if (finalRenderPos.y < 0)
			startY = 0;
		//If the rect is off of the right side of the screen offset the right side of the rectangle with the amount its off of the screen by
		if (finalRenderPos.x + size.x >= screenDims.width)
			endX = (screenDims.width - (int)finalRenderPos.x) * 4;
		//If the rect is off of the bottom side of the screen offset the bottom side of the rectangle with the amount its off of the screen by
		if (finalRenderPos.y + size.y >= screenDims.height)
			endY = endY - ((startY + (int)size.y) - screenDims.height);

		Vec2i spriteDims = sprites[Background]->GetSpriteDims();
		BYTE* imgPtr = sprites[Background]->GetImage();
		BYTE* screenPtr = screen->screenData;

		//Copies the background line by line to the screen using the dimentions of the local rectangle
		for (int y = startY; y < endY; ++y)
		{
			int offset = ((y * screenDims.width) + ((int)finalRenderPos.x + startX)) * 4;
			int backgroundOffset = ((y * spriteDims.width + (int)finalRenderPos.x + startX) * 4);
			memcpy(&screenPtr[offset], &imgPtr[backgroundOffset], endX);
		}
	}

	void RenderManager::Stop()
	{

		//Deletes the sprites from the vector and then clears it
		sprites.clear();

		for (auto s : spritesheets)
			s->Stop();

		//Deletes the spritesheets from the vector and then clears it
		spritesheets.clear();

		previousSprites.clear();
		previousSpritesheets.clear();

		//Delete the instance of the rendering manager
		delete instance;
	}

	//Clear the buffers that store sprites in the renderer
	void RenderManager::ClearRenderer()
	{
		previousSprites = sprites;
		previousSpritesheets = spritesheets;
		sprites.clear();
		spritesheets.clear();
	}

	//Reset the live sprite buffers to what they were before a reload occured
	void RenderManager::ResetToPrevious()
	{
		sprites = previousSprites;
		spritesheets = previousSpritesheets;
		previousSprites.clear();
		previousSpritesheets.clear();
	}

	//Clear the previous buffer of sprites - successful reload of levels
	void RenderManager::ClearPrevious()
	{
		previousSprites.clear();
		previousSpritesheets.clear();
	}

	void RenderManager::RenderToBackground(SmartPtr<Sprite> sprite, Vec3d renderPos)
	{
		Vec2i screenDims = sprites[Background]->GetSpriteDims();
		Vec2i spriteDims = sprite->GetSpriteDims();

		Vec2d finalRenderPos = Vec2d(renderPos.x, renderPos.y);
		finalRenderPos.x -= spriteDims.x / 2;
		finalRenderPos.y -= spriteDims.y / 2;

		//If the image is off of the screen dont execute the render function
		if (finalRenderPos.x >= screenDims.width || finalRenderPos.y >= screenDims.height || finalRenderPos.x + spriteDims.width < 0 || finalRenderPos.y + spriteDims.height < 0)
			return;

		//A local "rectangle" which handles image clipping
		int startX = 0;
		int startY = 0;
		int endX = 0;
		int endY = 0;


		BYTE* imgPtr = sprite->GetImage();
		BYTE* screenPtr = sprites[Background]->GetImage();

		//If the image is off of the left side of the screen offset the left side of the rectangle with the amount its off of the screen by
		if (finalRenderPos.x < 0)
			startX = abs((int)finalRenderPos.x);
		//If the image is off of the top side of the screen offset the top side of the rectangle with the amount its off of the screen by
		if (finalRenderPos.y < 0)
			startY = abs((int)finalRenderPos.y);
		//If the image is off of the right side of the screen offset the right side of the rectangle with the amount its off of the screen by
		if (finalRenderPos.x + spriteDims.width > screenDims.width)
			endX = (spriteDims.width + (int)finalRenderPos.x) - screenDims.width;
		//If the image is off of the bottom side of the screen offset the bottom side of the rectangle with the amount its off of the screen by
		if (finalRenderPos.y + spriteDims.height > screenDims.height)
			endY = ((int)finalRenderPos.y + spriteDims.height) - screenDims.height;

		//The rendering loop for each pixal of the images clipped rectangle
		for (int y = startY; y < spriteDims.height - endY; ++y)
		{
			for (int x = startX; x < spriteDims.width - endX; ++x)
			{
				//Offset of the current pixel for 4 BYTES (RGBA)
				int offset = ((y * spriteDims.width) + x) * 4;

				//Gets the alpha value from the images data by the offset for the alpha BYTE of the pixel
				BYTE alpha = imgPtr[offset + 3];

				//Coninue the next loop
				if (alpha <= 0)
					continue;

				//Sets a screen offset for where the BYTE data needs to be written to the screen
				int scrOffset = ((((int)finalRenderPos.y + y) * screenDims.width) + (((int)finalRenderPos.x + x))) * 4;

				//If the image doesn't have transparency copy the data from the sprites data to the screen for (RGB)
				//Note the 4th alpha BYTE of the screen is being used as a z-index
				if (alpha >= 255)
				{
					memcpy(&screenPtr[scrOffset], &imgPtr[offset], 3);
				}
				else
				{
					//Otherwise get each BYTE of colour from the image
					BYTE blue = imgPtr[offset];
					BYTE red = imgPtr[offset + 1];
					BYTE green = imgPtr[offset + 2];

					//And lerp the screens BYTE data with the sprites BYTE data using the amount of alpha as a percentage offset
					screenPtr[scrOffset] = screenPtr[scrOffset] + ((alpha * (blue - screenPtr[scrOffset])) >> 8);
					screenPtr[scrOffset + 1] = screenPtr[scrOffset + 1] + ((alpha*(red - screenPtr[scrOffset + 1])) >> 8);
					screenPtr[scrOffset + 2] = screenPtr[scrOffset + 2] + ((alpha*(green - screenPtr[scrOffset + 2])) >> 8);
				}
			}
		}
	}

	void RenderManager::DrawCollisionMask(int posX, int posY, DWORD* collisionMask, Rect sourceRct)
	{
		Vec2i finalRenderPos = Vec2i(posX, posY);
		finalRenderPos.x -= sourceRct.Width() / 2;
		finalRenderPos.y -= sourceRct.Height() / 2;

		// Set up start points
		BYTE *destPnter = screen->screenData + (finalRenderPos.x + finalRenderPos.y * screen->screenDimentions.width) * 4;
		int endOfLineDestIncrement = (screen->screenDimentions.width - sourceRct.Width()) * 4;

		DWORD *maskPointer = collisionMask;
		DWORD currentBit = 0;

		for (int y = 0; y<sourceRct.Height(); y++)
		{
			for (int x = 0; x<sourceRct.Width(); x++)
			{
				// Check current bit, white for set otherwise leave as destination
				if (*maskPointer & (1 << currentBit))
					memset(destPnter, 255, 4);

				currentBit++;
				if (currentBit == 32)
				{
					currentBit = 0;
					maskPointer++;
				}
				destPnter += 4;
			}

			destPnter += endOfLineDestIncrement;
		}
	}
}
