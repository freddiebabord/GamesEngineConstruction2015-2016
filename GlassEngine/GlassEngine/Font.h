#if !defined (FONT)
#define FONT

#include "precomp.h"

namespace GlassEngine{
	struct Font
	{
		HAPI_TColour fontColour = HAPI_TColour(255, 255, 255);
		int fontSize = 12;
		int fontWeight = 500;
		std::string fontName = "Resources\\Federation.ttf";
		bool antiAliasing = false;
	};
}
#endif