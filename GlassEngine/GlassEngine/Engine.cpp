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
		Animator.Start();
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
		Animator.Update();

		for (auto gos : Game.CurrentLevel()->GetGameObjects())
		{
			Vec3d pos = gos->GetTransform()->GetPosition();
			Vec2i size = Vec2i(0);
			if (gos->GetComponent<Sprite>(SpriteC))
			{
				if (gos->GetComponent<Sprite>(SpriteC)->Renderable())
					size = gos->GetComponent<Sprite>(SpriteC)->GetSpriteDims();
			}
			else if (gos->GetComponent<SpriteSheet>(SpriteSheetC))
				size = gos->GetComponent<SpriteSheet>(SpriteSheetC)->GetIdvSpriteDims();
			else
				continue;
			Renderer.AddDirtyRectangle(DirtyRectangle(Vec2i((int)pos.x, (int)pos.y), Vec2i((int)size.x, (int)size.y)));
		}

		if (fixedUpdateTime < HAPI->GetTime() - (1000 / 40))
		{
			fixedUpdateTime = HAPI->GetTime();
			Physics.FixedUpdate();
			for (auto gos : Game.CurrentLevel()->GetGameObjects())
			{
				if (gos->isActive())
				{
					for (auto gos2 : Game.CurrentLevel()->GetGameObjects())
					{
						if (gos2->isActive() && gos != gos2)
						{
							if (Physics.CheckForCollision(gos->GetTransform()->GetPosition(), gos->ColliderRef(), gos2->GetTransform()->GetPosition(), gos2->ColliderRef()))
							{
								if (!gos->Collided())
									gos->OnCollisionEnter(gos2);
								if (!gos2->Collided())
									gos2->OnCollisionEnter(gos);
								//HAPI->UserMessage("I've Collided!!!!!", "Glass Engine");
							}
						}
					}
				}
			}
			Game.FixedUpdate();
		}

#if defined(_DEBUG)
		Renderer.AddDirtyRectangle(DirtyRectangle(Vec2i(5, 5), Vec2i(150, 180)));

		if (Input.GetKey(HK_LSHIFT) && Input.GetKey('`')){
			Renderer.AddDirtyRectangle(DirtyRectangle(Vec2i(0), Vec2i(Renderer.GetScreenDimentions().width, 100)));
		}
#endif

		for (int i = 0; i < 4; ++i)
		{
			auto gameObject = Game.CurrentLevel()->GetGameObjects()[i];
			if (Input.WasControllerConnectedLastUpdadate(i))
				gameObject->isActive(true);
			else if (Input.WasControllerDisconnectedLastUpdate(gameObject->GetID()))
				gameObject->isActive(false);
		}

		for (auto gos : Game.CurrentLevel()->GetGameObjects())
		{
			if (gos->isActive())
			{
				if (gos->SpriteRef() > 0)
					Renderer.Render(gos->SpriteRef(), gos->GetTransform()->GetPosition());
				else if (gos->SpriteSheetRef() >= 0)
				{
					auto pos = gos->GetTransform()->GetPosition();
					auto sp = gos->GetComponent<Animation>(AnimationC)->GetCurrentSprite();
					Renderer.Render(gos->SpriteSheetRef(), pos, sp);
				}
			}
		}

		

		/*for (auto gos : Game.CurrentLevel()->GetUIObjects())
		{
			if (gos->isActive())
			{
				if (gos->GetComponent<Sprite>(SpriteC))
					Renderer.Render(gos->GetComponent<Sprite>(SpriteC), gos->GetTransform()->GetPosition());
				else if (gos->GetComponent<SpriteSheet>(SpriteSheetC))
				{
					auto pos = gos->GetTransform()->GetPosition();
					auto sp = gos->GetComponent<SpriteSheet>(SpriteSheetC)->GetCurrentSprite();
					Renderer.Render(gos->GetComponent<SpriteSheet>(SpriteSheetC), pos, sp);
				}
			}
		}*/

		
		//UI.Update();
		if (Input.GetKeyUp(HK_ESCAPE))
		{
			//UI.GetUIObjects()
			HAPI->Close();
		}
	}

	void Engine::Stop()
	{
		Game.Stop();
		Physics.Stop();
		UI.Stop();
		Animator.Stop();
		Renderer.Stop();
		Input.Stop();
		Time.Stop();
		delete instance;
	}

}
