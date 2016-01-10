#if !defined GLASS_SPRITE
#define GLASS_SPRITE

#include "GlassObject.h"

class GlassSprite :	public GlassObject
{
public:
	GlassSprite(std::string dataPath, Vector3 pos);
	~GlassSprite();

	BYTE* imageData;
	int width, height = 0;
};

#endif

