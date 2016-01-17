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
// TODO: Remove circular shared-ptr ref by using weak ptr
// TODO: Create an actual heirachichal node like gameobject class
// http://programmers.stackexchange.com/questions/274801/raw-weak-ptr-unique-ptr-shared-ptr-etc-how-to-choose-them-wisely


using namespace GlassEngine;

void HAPI_Main()
{
#if defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(1936);
	/*_CrtSetBreakAlloc(1921);
	_CrtSetBreakAlloc(1918);
	_CrtSetBreakAlloc(1917);
	_CrtSetBreakAlloc(1916);
	_CrtSetBreakAlloc(1915);
	_CrtSetBreakAlloc(1914);
	_CrtSetBreakAlloc(1913);

	_CrtSetBreakAlloc(1846);
	_CrtSetBreakAlloc(1843);
	_CrtSetBreakAlloc(1842);
	_CrtSetBreakAlloc(1841);
	_CrtSetBreakAlloc(1840);
	_CrtSetBreakAlloc(1839);
	_CrtSetBreakAlloc(1838);
	
	_CrtSetBreakAlloc(1771);
	_CrtSetBreakAlloc(1768);
	_CrtSetBreakAlloc(1767);
	_CrtSetBreakAlloc(1766);
	_CrtSetBreakAlloc(1765);
	_CrtSetBreakAlloc(1764);
	_CrtSetBreakAlloc(1763);
	
	_CrtSetBreakAlloc(1696);
	_CrtSetBreakAlloc(1693);
	_CrtSetBreakAlloc(1692);
	_CrtSetBreakAlloc(1691);
	_CrtSetBreakAlloc(1690);
	_CrtSetBreakAlloc(1689);
	_CrtSetBreakAlloc(1688);*/
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