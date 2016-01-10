#include "precomp.h"

#if defined(DEBUG) | defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK,__FILE__, __LINE__)
#endif

#include "Engine.h"

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