#pragma once
#include "GlassHeader.h"

class GlassGraphics
{
public:
	GlassGraphics();
	~GlassGraphics();

	GlassGraphics(BYTE* screen);

	void ClearScreen(BYTE* screenData, int width, int height);
	void GetDesktopResolution(int& screenWidth, int& screenHeight);
	
	virtual void Render(BYTE* screenData);

	BYTE* screen = HAPI->GetScreenPointer();
	const HAPI_TColour backgroundColour = HAPI_TColour(0, 0, 255, 0);
	
	int screenWidth;
	int screenHeight;
};

