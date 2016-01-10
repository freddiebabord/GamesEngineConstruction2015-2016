#include "GlassSprite.h"


GlassSprite::GlassSprite(std::string dataPath, Vector3 pos)
{
	if (!HAPI->LoadTexture(dataPath, &imageData, &width, &height))
	{
		
		HAPI_TColour errorImage = HAPI_TColour(255, 20, 147);
		width = 50;
		height = 50;
		imageData = new BYTE[50 * 50 * 4];
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				memcpy(&imageData[(x + (y*width)) * 4], &errorImage, 4);
			}
		}

		/*
		HAPI->UserMessage("Error: File not found", "Glass Engine");
		HAPI->Close();*/
	}
	positon = pos;
}


GlassSprite::~GlassSprite()
{
	delete[] imageData;
}
