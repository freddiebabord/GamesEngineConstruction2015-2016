#include "precomp.h"
#include "Level.h"

namespace GlassEngine{

	void Level::DeleteLevel()
	{
		for (auto g : gameObjects)
			g->Destory();
		gameObjects.clear();
	}
}