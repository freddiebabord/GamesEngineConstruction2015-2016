#include "precomp.h"
#include "Engine.h"
#include "GameObject.h"
#include "Sprite.h"
#include "SpriteSheet.h"
#include "Transform.h"
#include "UIObject.h"

namespace GlassEngine{


	Engine *Engine::instance{ nullptr };

	Engine &Engine::Instance()
	{
		if (instance == nullptr)
			instance = new Engine;
		return *instance;
	}
	
	void Engine::Start(bool debugMode, bool fullscreen, int width, int height)
	{
		Time.Start();
		Renderer.Start(width,height,fullscreen);
		UI.Start();
		Physics.Start();
		Input.Start();
		Game.Start();
		if (debugMode)
			HAPI->SetShowFPS(true, 50, 50);
		fixedUpdateTime = HAPI->GetTime();

	}

	void Engine::Update()
	{
		Time.Update();
		Input.Update();		
		Physics.Update();
		Game.Update();
		Renderer.Update();
		//Renderer.Render(Background, Vec3d(0.0));
		
		for (auto gos : Game.CurrentLevel()->GetGameObjects())
		{
			Vec3d pos = gos->GetTransform()->GetPosition();
			Vec2i size = Vec2i(0);
			if (gos->GetSprite())
				size = gos->GetSprite()->GetSpriteDims();
			else if (gos->GetSpritesheet())
				size = gos->GetSpritesheet()->GetIdvSpriteDims();
			Renderer.RenderDR(Vec2i(pos.x - 5, pos.y - 5), Vec2i(size.x + 10, size.y + 10));
		}

		if (fixedUpdateTime < HAPI->GetTime() - (1000 / 40))
		{
			fixedUpdateTime = HAPI->GetTime();
			Physics.FixedUpdate();
			Game.FixedUpdate();

		}
#if defined(_DEBUG)
			Renderer.RenderDR(Vec2i(5, 5), Vec2i(150, 80));

			if (Input.GetKey(HK_LSHIFT) && Input.GetKey('`'))
				Renderer.RenderDR(Vec2i(0), Vec2i(Renderer.GetScreenDimentions().width, 100));
#endif
			

			for (auto gos : Game.CurrentLevel()->GetGameObjects())
			{
				if (gos->GetID() < 4)
				{
					if (Input.WasControllerConnectedLastUpdadate(gos->GetID()))
					{
						gos->isActive(true);
						if (gos->GetSprite())
							Renderer.Render(gos->GetSprite(), gos->GetTransform()->GetPosition());
						else if (gos->GetSpritesheet())
							Renderer.Render(gos->GetSpritesheet(), gos->GetTransform()->GetPosition(), gos->GetSpritesheet()->GetCurrentSprite());
					}
					else if (Input.WasControllerDisconnectedLastUpdate(gos->GetID()))
					{
						gos->isActive(false);
						Vec3d pos = gos->GetTransform()->GetPosition();
						Vec2i size = Vec2i(0);

						if (gos->GetSprite())
							size = gos->GetSprite()->GetSpriteDims();
						else if (gos->GetSpritesheet())
							size = gos->GetSpritesheet()->GetIdvSpriteDims();
						else
							size = Vec2i(100, 100);
						Renderer.RenderDR(Vec2i(pos.x - 5, pos.y - 5), Vec2i(size.x + 10, size.y + 10));
					}
				}
				else
				{
					if (gos->GetSprite())
						Renderer.Render(gos->GetSprite(), gos->GetTransform()->GetPosition());
					else if (gos->GetSpritesheet())
						Renderer.Render(gos->GetSpritesheet(), gos->GetTransform()->GetPosition(), gos->GetSpritesheet()->GetCurrentSprite());

				}
			}

			for (auto gos : Game.CurrentLevel()->GetGameObjects())
			{
				if (gos->isActive())
				{
					if (gos->GetSprite())
						Renderer.Render(Player, gos->GetTransform()->GetPosition());
					else if (gos->GetSpritesheet())
					{
						auto pos = gos->GetTransform()->GetPosition();
						auto sp = gos->GetSpritesheet()->GetCurrentSprite();
						Renderer.Render(0, pos, sp);
					}
				}
			}
		
		if (UI.GetUIObjects().size() > 0)
		{
			for (auto uio : UI.GetUIObjects())
			{
				Renderer.Render(uio->currentSprite, Vec3d(uio->rect.GetRectDims().left, uio->rect.GetRectDims().top, 0));
			}
		}
		UI.Update();
		if (Input.GetKeyUp(HK_ESCAPE))
		{
			HAPI->Close();
		}
	}

	void Engine::Stop()
	{
		Game.Stop();
		Physics.Stop();
		UI.Stop();
		Renderer.Stop();
		Input.Stop();
		Time.Stop();
		delete instance;
	}

}
