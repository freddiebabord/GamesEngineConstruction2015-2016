#include "precomp.h"
#include "RenderManager.h"
#include "Sprite.h"
#include "SpriteSheet.h"
#include "Transform.h"
#include "GameObject.h"

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
		if (!HAPI->Initialise(&screenWidth, &screenHeight))
		{
			return;
		}

		//Create new unique screen data pointer
		screen = std::make_unique<Screen>(screenWidth, screenHeight, HAPI->GetScreenPointer());
	}

	void RenderManager::Update()
	{
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
	void RenderManager::Render(const int& spriteIndex, const Vec3i& renderPos)
	{
		Vec2i screenDims = screen->screenDimentions;
		Vec2i spriteDims = sprites[spriteIndex]->GetSpriteDims();

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
				if (alpha == 0)
					continue;

				//Sets a screen offset for where the BYTE data needs to be written to the screen
				int scrOffset = (((renderPos.y + y) * screenDims.width) + ((renderPos.x + x))) * 4;

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

		if (spriteIndex != Background)
		{
			for (auto c : sprites[spriteIndex]->GetParent()->GetChildren())
			{
				if (c->GetSprite())
					Render(c->GetSprite(), c->GetTransform()->GetPosition());
				else if (c->GetSpritesheet())
					Render(c->GetSpritesheet(), c->GetTransform()->GetPosition(), c->GetSpritesheet()->GetCurrentSprite());
			}
		}
	};

	void RenderManager::Render(const std::shared_ptr<Sprite> sprite, const Vec3i& renderPos)
	{
		Vec2i screenDims = screen->screenDimentions;
		Vec2i spriteDims = sprite->GetSpriteDims();

		//If the image is off of the screen dont execute the render function
		if (renderPos.x >= screenDims.width || renderPos.y >= screenDims.height || renderPos.x + spriteDims.width < 0 || renderPos.y + spriteDims.height < 0)
			return;

		//A local "rectangle" which handles image clipping
		int startX = 0;
		int startY = 0;
		int endX = 0;
		int endY = 0;


		BYTE* imgPtr = sprite->GetImage();
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
				if (alpha == 0)
					continue;

				//Sets a screen offset for where the BYTE data needs to be written to the screen
				int scrOffset = (((renderPos.y + y) * screenDims.width) + ((renderPos.x + x))) * 4;

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
		if (sprite != sprites[Background])
		{
			for (auto c : sprite->GetParent()->GetChildren())
			{
				if (c->GetSprite())
					Render(c->GetSprite(), c->GetTransform()->GetPosition());
				else if (c->GetSpritesheet())
					Render(c->GetSpritesheet(), c->GetTransform()->GetPosition(), c->GetSpritesheet()->GetCurrentSprite());
			}
		}
	};

	//Render an image on screen
	//int spriteIndex: The index of the spritesheet in the render managers vector of spritesheet 
	//int idvSpriteIndex: The index of the sprite frame for the spritesheet 
	//Vec3 renderPos: The position in 3D space the image should be rendered at
	void RenderManager::Render(const int& spriteIndex, const Vec3i& renderPos, const int& idvSpriteIndex)
	{
		//The current spriesheet from the index
		std::shared_ptr<SpriteSheet> sheet = spritesheets[spriteIndex];

		Vec2i screenDims = screen->screenDimentions;
		Vec2i spriteIdvDims = sheet->GetIdvSpriteDims();
		Vec2i spriteDims = sheet->GetSpriteDims();

		//The local X, Y on the sprite sheet
		int spriteY = idvSpriteIndex / (spriteDims.width / spriteIdvDims.width);
		int spriteX = idvSpriteIndex % (spriteDims.width / spriteIdvDims.width);

		//if the sprite requeststed is off of the sheet dont contiue the function
		if (spriteX >= spriteDims.width / spriteIdvDims.width ||
			spriteY >= spriteDims.height / spriteIdvDims.height)
			return;

		//if the rendered position is off of the screen dont render
		if (renderPos.x >= screenDims.width ||
			renderPos.y >= screenDims.height ||
			renderPos.x + spriteIdvDims.width < 0 ||
			renderPos.y + spriteIdvDims.height < 0)
			return;

		//A local "rectangle" which handles image clipping
		int startX = 0;
		int startY = 0;
		int endX = 0;
		int endY = 0;

		//If the image is off of the left side of the screen offset the left side of the rectangle with the amount its off of the screen by
		if (renderPos.x < 0)
			startX = abs((int)renderPos.x); 
		//If the image is off of the top side of the screen offset the top side of the rectangle with the amount its off of the screen by
		if (renderPos.y < 0)
			startY = abs((int)renderPos.y);
		//If the image is off of the right side of the screen offset the right side of the rectangle with the amount its off of the screen by
		if (renderPos.x + spriteIdvDims.width > screenDims.width)
			endX = (spriteIdvDims.width + (int)renderPos.x) - screenDims.width;
		//If the image is off of the bottom side of the screen offset the bottom side of the rectangle with the amount its off of the screen by
		if (renderPos.y + spriteIdvDims.height > screenDims.height)
			endY = ((int)renderPos.y + spriteIdvDims.height) - screenDims.height;

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
				if (alpha == 0) continue;

				int scrOffset = (((int)(renderPos.y + y)  * screenDims.width) + ((int)(renderPos.x + x))) * 4;

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

		for (auto c : sheet->GetParent()->GetChildren())
		{
			if (c->GetSprite())
				Render(c->GetSprite(), c->GetTransform()->GetPosition());
			else if (c->GetSpritesheet())
				Render(c->GetSpritesheet(), c->GetTransform()->GetPosition(), c->GetSpritesheet()->GetCurrentSprite());
		}
	};

	void RenderManager::Render(const std::shared_ptr<SpriteSheet> sheet, const Vec3i& renderPos, const int& idvSpriteIndex)
	{

		Vec2i screenDims = screen->screenDimentions;
		Vec2i spriteIdvDims = sheet->GetIdvSpriteDims();
		Vec2i spriteDims = sheet->GetSpriteDims();

		//The local X, Y on the sprite sheet
		int spriteY = idvSpriteIndex / (spriteDims.width / spriteIdvDims.width);
		int spriteX = idvSpriteIndex % (spriteDims.width / spriteIdvDims.width);

		//if the sprite requeststed is off of the sheet dont contiue the function
		if (spriteX >= spriteDims.width / spriteIdvDims.width ||
			spriteY >= spriteDims.height / spriteIdvDims.height)
			return;

		//if the rendered position is off of the screen dont render
		if (renderPos.x >= screenDims.width ||
			renderPos.y >= screenDims.height ||
			renderPos.x + spriteIdvDims.width < 0 ||
			renderPos.y + spriteIdvDims.height < 0)
			return;

		//A local "rectangle" which handles image clipping
		int startX = 0;
		int startY = 0;
		int endX = 0;
		int endY = 0;

		//If the image is off of the left side of the screen offset the left side of the rectangle with the amount its off of the screen by
		if (renderPos.x < 0)
			startX = abs((int)renderPos.x);
		//If the image is off of the top side of the screen offset the top side of the rectangle with the amount its off of the screen by
		if (renderPos.y < 0)
			startY = abs((int)renderPos.y);
		//If the image is off of the right side of the screen offset the right side of the rectangle with the amount its off of the screen by
		if (renderPos.x + spriteIdvDims.width > screenDims.width)
			endX = (spriteIdvDims.width + (int)renderPos.x) - screenDims.width;
		//If the image is off of the bottom side of the screen offset the bottom side of the rectangle with the amount its off of the screen by
		if (renderPos.y + spriteIdvDims.height > screenDims.height)
			endY = ((int)renderPos.y + spriteIdvDims.height) - screenDims.height;

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
				if (alpha == 0) continue;

				int scrOffset = (((int)(renderPos.y + y)  * screenDims.width) + ((int)(renderPos.x + x))) * 4;

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

		for (auto c : sheet->GetParent()->GetChildren())
		{
			if (c->GetSprite())
				Render(c->GetSprite(), c->GetTransform()->GetPosition());
			else if (c->GetSpritesheet())
				Render(c->GetSpritesheet(), c->GetTransform()->GetPosition(), c->GetSpritesheet()->GetCurrentSprite());
		}
	};

	//Returns a Vector2<int> of the screen dimentions
	const Vec2i RenderManager::GetScreenDimentions()
	{
		return screen->screenDimentions;
	}

	// Add a sprite to the render manager by loading in a sprite from a specified path string
	bool RenderManager::AddSprite(const std::string& path)
	{
		sprites.push_back(std::make_shared<Sprite>(path));
		return true;
	}

	// Add a sprite to the render manager by a pointer to a block of memory which contains the sprite
	bool RenderManager::AddSprite(std::shared_ptr<Sprite> spriteToAdd)
	{
		sprites.push_back(spriteToAdd);
		return true;
	}

	// Add a spritesheet to the render manager by loading in a sprite from a specified path string
	bool RenderManager::AddSpriteSheet(const std::string& path, Vec2i spriteSize)
	{
		spritesheets.push_back(std::make_shared<SpriteSheet>(path, spriteSize));
		return true;
	}

	// Add a spritesheet to the render manager by a pointer to a block of memory which contains the sprite
	bool RenderManager::AddSpriteSheet(std::shared_ptr<SpriteSheet> spriteToAdd)
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
		//If the position is off the screen then dont dirty rect
		if (pos.x >= screenDims.width || pos.y >= screenDims.height || pos.x + size.x < 0 || pos.y + size.y < 0)
			return;

		//A local "rectangle" which handles image clipping
		int startX = 0;
		int startY = (int)pos.y;
		int endX = (int)size.x * 4;
		int endY = (int)pos.y + (int)size.y;

		//If the rect is off of the left side of the screen offset the left side of the rectangle with the amount its off of the screen by
		if (pos.x < 0)
			startX = abs((int)pos.x);
		//If the rect is off of the top side of the screen offset the top side of the rectangle with the amount its off of the screen by
		if (pos.y < 0)
			startY = 0;
		//If the rect is off of the right side of the screen offset the right side of the rectangle with the amount its off of the screen by
		if (pos.x + size.x >= screenDims.width)
			endX = (screenDims.width - (int)pos.x) * 4;
		//If the rect is off of the bottom side of the screen offset the bottom side of the rectangle with the amount its off of the screen by
		if (pos.y + size.y >= screenDims.height)
			endY = endY - ((startY + (int)size.y) - screenDims.height);

		Vec2i spriteDims = sprites[Background]->GetSpriteDims();
		BYTE* imgPtr = sprites[Background]->GetImage();
		BYTE* screenPtr = screen->screenData;

		//Copies the background line by line to the screen using the dimentions of the local rectangle
		for (int y = startY; y < endY; ++y)
		{
			int offset = ((y * screenDims.width) + ((int)pos.x + startX)) * 4;
			int backgroundOffset = ((y * spriteDims.width + (int)pos.x + startX) * 4);
			memmove(&screenPtr[offset], &imgPtr[backgroundOffset], endX);
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
}
