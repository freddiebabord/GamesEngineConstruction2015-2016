#include "precomp.h"
#include "Engine.h"
#include "RenderManager.h"
#include "Animator.h"
#include "InputManager.h"
#include "PhysicsManager.h"
#include "UIManager.h"
#include "Time.h"
#include "Game.h"
#include "GameObject.h"
#include "Sprite.h"
#include "SpriteSheet.h"
#include "Transform.h"
#include "UIObject.h"
#include "Animation.h"
#include "SpriteCollider.h"
#include "GamePlayerHeaders.h"

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
		Animator.Start();
		if (debugMode)
			HAPI->SetShowFPS(true, 50, 50);
		fixedUpdateTime = HAPI->GetTime();

		//Game.ReloadGame();
	}

	void Engine::Update()
	{
		Time.Update();
		Input.Update();

		if (fixedUpdateTime < HAPI->GetTime() - (1000 / (90 * Time.TimeScale())))
		{

			Physics.Update();
			if (Game.CurrentLevel()->GetID() > 0)
			{
				Renderer.Update();
				Animator.Update();


				gameObjects_ = Game.CurrentLevel()->GetGameObjects();

				for (auto gos : gameObjects_)
				{
					Vec3d pos = gos->GetTransform()->GetPosition();
					Vec2i size = Vec2i(0);
					if (gos->SpriteRef() > 0)
					{
						if (Renderer.GetSprite(gos->SpriteRef())->Renderable())
							size = Renderer.GetSprite(gos->SpriteRef())->GetSpriteDims();
					}
					else if (gos->SpriteSheetRef() >= 0)
						size = Renderer.GetSpriteSheet(gos->SpriteSheetRef())->GetIdvSpriteDims();
					else
						continue;
					Renderer.AddDirtyRectangle(DirtyRectangle(Vec2i((int)pos.x, (int)pos.y), Vec2i((int)size.x, (int)size.y)));
				}

				if (fixedUpdateTime < HAPI->GetTime() - (1000 / (40 * Time.TimeScale())))
				{
					fixedUpdateTime = HAPI->GetTime();
					Physics.FixedUpdate();

					Game.FixedUpdate();
				}

#if defined(_DEBUG)
				Renderer.AddDirtyRectangle(DirtyRectangle(Vec2i(5, 5), Vec2i(150, 250)));

				if (Input.GetKey(HK_LSHIFT) && Input.GetKey('`')){
					Renderer.AddDirtyRectangle(DirtyRectangle(Vec2i(0), Vec2i(Renderer.GetScreenDimentions().width, 100)));
					if (Input.GetButton(HK_DIGITAL_B, 0))
						showMask = !showMask;
				}
#endif

				for (int i = 0; i < 4; ++i)
				{
					auto gameObject = gameObjects_[i];

					//if (!gameObject->isActive()) continue;

					if (Input.WasControllerConnectedLastUpdadate(i))
						gameObject->isActive(true);
					else if (Input.WasControllerDisconnectedLastUpdate(gameObject->GetID()))
						gameObject->isActive(false);
				}

				for (auto gos : gameObjects_)
				{
					if (!gos->isActive() || gos->GetName() == "") continue;

					if (gos->SpriteRef() > 0)
						Renderer.Render(gos->SpriteRef(), gos->GetTransform()->GetPosition());
					else if (gos->SpriteSheetRef() >= 0)
					{
						auto pos = gos->GetTransform()->GetPosition();
						auto sp = gos->GetComponent<Animation>(AnimationC)->GetCurrentSprite();
						Renderer.Render(gos->SpriteSheetRef(), pos, sp);
					}

					for (auto child : gos->GetChildren())
					{
						if (!child->isActive() || child->GetName() == "") continue;

						if (child->SpriteRef() > 0)
							Renderer.Render(child->SpriteRef(), child->GetTransform()->GetPosition());
						else if (child->SpriteSheetRef() >= 0)
						{
							auto pos = child->GetTransform()->GetPosition();
							auto sp = child->GetComponent<Animation>(AnimationC)->GetCurrentSprite();
							Renderer.Render(child->SpriteSheetRef(), pos, sp);
						}
					}
				}

			}

			Game.Update();

			UI.Update();

			if (Game.IsGameOver())
			{
				UI.Reset();
				Renderer.ClearRenderer();
				Physics.Reset();
				Input.Reset();
				UI.loadNewLevel = false;
				Game.DeleteLevels();
				UI.ConfirmReset();
				Renderer.ClearPrevious();
				Physics.ClearPrevious();
				auto level = MakeSmartPtr<MainMenu>(Game.GetScore());
				level->OnStart();
				Game.AddLevel(level);
				Game.SetGameOver(false);
				Game.Reset();
			}
		}

		if (Input.GetKeyUp(HK_ESCAPE))
		{
			HAPI->Close();
		}
	}

	void Engine::Stop()
	{
		Animator.Stop();
		Game.Stop();
		Physics.Stop();
		UI.Stop();
		Renderer.Stop();
		Input.Stop();
		Time.Stop();
		delete instance;
	}

}
