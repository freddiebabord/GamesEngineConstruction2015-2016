#include "GlassGraphics.h"

GlassGraphics::GlassGraphics()
{

}

GlassGraphics::~GlassGraphics()
{
	delete[] screen;
}

GlassGraphics::GlassGraphics(BYTE* screen)
{
	ClearScreen(screen, screenWidth, screenHeight);
}

void GlassGraphics::ClearScreen(BYTE* screenData, int width, int height)
{
	for (int i = 0; i < (width * height) * 4; i+=4)
	{
		screenData[i] = (BYTE)backgroundColour.alpha;
		screenData[i+1] = (BYTE)backgroundColour.red;
		screenData[i+2] = (BYTE)backgroundColour.green;
		screenData[i+3] = (BYTE)backgroundColour.blue;
	}
}


void GlassGraphics::GetDesktopResolution(int& screenWidth, int& screenHeight)
{
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	screenWidth = desktop.right;
	screenHeight = desktop.bottom;
}