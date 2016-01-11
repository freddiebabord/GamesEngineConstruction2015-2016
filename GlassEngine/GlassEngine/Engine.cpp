#include "precomp.h"
#include "Engine.h"
#include "GameObject.h"
#include "Sprite.h"
#include "SpriteSheet.h"
#include "Transform.h"

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
		Physics.Start();
		Input.Start();
		Game.Start();
		if (debugMode)
			HAPI->SetShowFPS(true, 50, 50);
		
		//assert(!Renderer.AddSprite(new Sprite("Resources/background.jpg")));
		//Renderer.AddSprite(new Sprite("Resources/background.jpg"));
		//Renderer.AddSprite(new Sprite("Resources/shield.png"));
		////Renderer.AddSprite(new Sprite("Resources/GalaxySpriteSheet.png"));
		//Renderer.AddSpriteSheet(new SpriteSheet("Resources/GalaxySpriteSheet.png", Vec2i(250,200)));

		//Renderer.AddSpriteSheet(new SpriteSheet("Resources/explosion.PNG", Vec2i(64, 64)));

		//Renderer.GetSpriteSheet(0)->AddAnimation(Animation(Vec2i(0, 4))); //IDL
		//Renderer.GetSpriteSheet(0)->AddAnimation(Animation(Vec2i(5, 12))); //FWD
		//Renderer.GetSpriteSheet(0)->AddAnimation(Animation(Vec2i(13, 14))); //RIGHT
		//Renderer.GetSpriteSheet(0)->AddAnimation(Animation(Vec2i(15, 16))); //LEFT
		//Renderer.GetSpriteSheet(0)->AddAnimation(Animation(Vec2i(17, 22))); //BACK

		//Renderer.GetSpriteSheet(1)->AddAnimation(Animation(Vec2i(0, 25))); //BACK

		//Renderer.Render(Background, Vec3i(0, 0, 0));
		//

		//for (int i = 0; i < 50; i++)
		//{
		//	frame.push_back(0);
		//	position.push_back(Vec3i(rand() % (Renderer.GetScreenDimentions().width+20)-50,
		//							 rand() % (Renderer.GetScreenDimentions().height+20)-50, 0));
		//	animation_.push_back(rand() % 4);
		//	time.push_back(HAPI->GetTime());
		//}
		//frame.push_back(0); 
		//time.push_back(HAPI->GetTime());
	}

	void Engine::Update()
	{
		Time.Update();
		Input.Update();		
		Physics.Update();
		Game.Update();

#if defined(_DEBUG)
		Renderer.RenderDR(Vec2i(5, 5), Vec2i(150, 80));

		if (Input.GetKey(HK_LSHIFT) && Input.GetKey('`'))
			Renderer.RenderDR(Vec2i(0), Vec2i(Renderer.GetScreenDimentions().width, 100));
#endif
		for (auto gos : Game.CurrentLevel()->GetGameObjects())
		{
			if (Input.ControllerIDExists(gos->GetID()))
			{
				Vec3i pos = gos->GetTransform()->GetPosition();
				Vec2i size = Vec2i(0);
				if (gos->GetSprite())
					size = gos->GetSprite()->GetSpriteDims();
				else if (gos->GetSpritesheet())
					size = gos->GetSpritesheet()->GetIdvSpriteDims();
				Renderer.RenderDR(Vec2i(pos.x - 5, pos.y - 5), Vec2i(size.x + 10, size.y + 10));
			}
		}

		for (auto gos : Game.CurrentLevel()->GetGameObjects())
		{
			if (Input.WasControllerConnectedLastUpdadate(gos->GetID()))
			{
				gos->isActive(true);
				if (gos->GetSprite())
					Renderer.Render(Player, gos->GetTransform()->GetPosition());
				else if (gos->GetSpritesheet())
					Renderer.Render(0, gos->GetTransform()->GetPosition(), gos->GetSpritesheet()->GetCurrentSprite());
			}
			else if (Input.WasControllerDisconnectedLastUpdate(gos->GetID()))
			{
				gos->isActive(false);
				Vec3i pos = gos->GetTransform()->GetPosition();
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
		


		for (auto gos : Game.CurrentLevel()->GetGameObjects())
		{
			if (gos->isActive())
			{
				if (gos->GetSprite())
					Renderer.Render(Player, gos->GetTransform()->GetPosition());
				else if (gos->GetComponent(SpriteSheetC))
					Renderer.Render(0, gos->GetTransform()->GetPosition(), gos->GetSpritesheet()->GetCurrentSprite());
			}
		}

		if (Input.GetKeyUp(HK_ESCAPE))
		{
			HAPI->Close();
		}
	}

	void Engine::Stop()
	{
		Game.Stop();
		Input.Stop();
		Physics.Stop();
		Renderer.Stop();
		Time.Stop();
		delete instance;
	}

}
