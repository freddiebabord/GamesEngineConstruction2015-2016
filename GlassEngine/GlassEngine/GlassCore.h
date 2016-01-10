#pragma once
#if !defined GLASS_CORE
#define GLASS_CORE

#include <Windows.h>
#include "wtypes.h"
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <math.h>


#include "HAPI/HAPI_InputCodes.h"
#include <HAPI_lib.h>
#include "GlassMaths.h"

// Clear the screen to a blanket colour by copying all of the pixels that were already set and then pasting them into the next set of the array. Fills the screen exponentially.
void ClearScreen(BYTE* screen, const int& screenWidth, const int& screenHeight, HAPI_TColour backgroundColour)
{
	//Set the first pixel to be that of the background colour
	memcpy(screen, &backgroundColour, 4);

	int currentPixel = 0;

	//Copy the pixels in powers of 2 so it fills more than half of the screen
	for (int i = 1; i * 2 < screenWidth*screenHeight; i *= 2)
	{
		memcpy(screen + i * 4, screen, i * 4);
		currentPixel = i;
	}

	//Find the remaining amount of pixels
	int remainingPixels = (screenWidth*screenHeight) - currentPixel;

	//copy half of the screen to the latter half - this will overwrite some existing pixels
	for (int i = (screenWidth * screenHeight) / 2; i < screenWidth*screenHeight; i *= 2)
	{
		memcpy(screen + i * 4, screen, i * 4);
		currentPixel = i;
	}
}

//Get the dimentions of the window (width and height)
void GetDesktopResolution(int& screenWidth, int& screenHeight, HWND& hDesktop)
{
	RECT desktop;
	hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	screenWidth = desktop.right;
	screenHeight = desktop.bottom;

}

void SetPixelColour(BYTE *screen, int screenWidth, int screenHeight, int x, int y, HAPI_TColour colour)
{
	int offset = (x + y * screenWidth) * 4;
	screen[offset] = colour.red;
	screen[offset + 1] = colour.green;
	screen[offset + 2] = colour.blue;
}

void Render(BYTE *screen, int screenWidth, int screenHeight, int width, int height, BYTE *imageData, Vector3 renderPos)
{
	
	BYTE* imgPtr = imageData;
	
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			
			BYTE alpha = imgPtr[3];
			
			int offset = (int)(renderPos.x + x + (renderPos.y +  y) * screenWidth) * 4;
			if (alpha > 0 && (offset < screenWidth * screenHeight * 4 && offset >= 0))
			{
				BYTE red = imgPtr[2];
				BYTE green = imgPtr[1];
				BYTE blue = imgPtr[0];	

				

				if (alpha >= 255){
					memcpy(&screen[offset], &imageData[(x + y * width) * 4], 4);
				}
				else
				{
					float mod = alpha / 255.f;
					
					screen[offset-1] = (BYTE)(mod * red + (1.0f - mod) * screen[offset-1]);
					screen[offset] = (BYTE)(mod * green + (1.0f - mod) * screen[offset]);
					screen[offset + 1] = (BYTE)(mod * blue + (1.0f - mod) * screen[offset + 1]);
				}
			}

			imgPtr += 4;
		}
	}
}


#endif