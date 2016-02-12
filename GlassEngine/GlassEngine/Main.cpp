#include "precomp.h"
#include "Engine.h"
//#include <vld.h>
#if defined(DEBUG) | defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
//#define new new(_NORMAL_BLOCK,__FILE__, __LINE__)
#endif

// TODO: Comment everything
// TODO: Finish AI
// TODO: Fix Constant Collisions and spawning of explositons / bullets

using namespace GlassEngine;

void HAPI_Main()
{
#if defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(1936);
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