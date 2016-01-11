#include "precomp.h"
#include "Engine.h"

#if defined(DEBUG) | defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK,__FILE__, __LINE__)
#endif


// TODO: Fix issue with getting spritesheet....
// TODO: Fix memory leak issue (probably from gameobjects not being deleted properly
// TODO: Comment everything

using namespace GlassEngine;

void HAPI_Main()
{
#if defined(_DEBUG)
	GEngine.Start(true,false, 1900, 1000);
#else
	GEngine.Start();
#endif

	while (HAPI->Update())
	{
		GEngine.Update();
	}

	GEngine.Stop();
}