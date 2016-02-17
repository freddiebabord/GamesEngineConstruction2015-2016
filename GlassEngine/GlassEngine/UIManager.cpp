#include "precomp.h"
#include "UIManager.h"
#include "Text.h"
#include "GameObject.h"
#include "Transform.h"
#include "RenderManager.h"
#include "Button.h"
#include "Game.h"

namespace GlassEngine
{
	UIManager *UIManager::instance{ nullptr };

	UIManager &UIManager::Instance()
	{
		if (instance == nullptr)
			instance = new UIManager;
		return *instance;
	}

	void UIManager::Start()
	{
		for (int i = 0; i < 4; ++i)
			PlayerCascades.push_back(false);
	}
	
	void UIManager::Update()
	{
		if (scoreText == nullptr && killCountText == nullptr)
		{
			for (auto ui : uiObjects)
			{
				if (ui->GetName() == "ScoreText")
					scoreText = std::static_pointer_cast<Text, UIObject>(ui);
				if (ui->GetName() == "EnemiesKilledText")
					killCountText = std::static_pointer_cast<Text, UIObject>(ui);
				if (ui->GetName() == "DifficultyText")
					difficultyText = std::static_pointer_cast<Text, UIObject>(ui);
				if (ui->GetName() == "Player1PhaserCascadeLabel")
					player1CascadeText = std::static_pointer_cast<Text, UIObject>(ui);
				if (ui->GetName() == "Player2PhaserCascadeLabel")
					player2CascadeText = std::static_pointer_cast<Text, UIObject>(ui);
				if (ui->GetName() == "Player3PhaserCascadeLabel")
					player3CascadeText = std::static_pointer_cast<Text, UIObject>(ui);
				if (ui->GetName() == "Player4PhaserCascadeLabel")
					player4CascadeText = std::static_pointer_cast<Text, UIObject>(ui);
			}
		}

		if (scoreText != nullptr)
			scoreText->Content("Score: " + std::to_string(Game.GetScore()));
		
		if (killCountText != nullptr)
			killCountText->Content("Ships Destroyed: " + std::to_string(Game.GetKillCount()));



		if (player1CascadeText != nullptr)
		{
			if (PlayerCascades[0])
				player1CascadeText->Content("Phaser Cascade: ONLINE");
			else
				player1CascadeText->Content("Phaser Cascade: OFFLINE");
		}

		if (player2CascadeText != nullptr)
		{
			if (PlayerCascades[1])
				player2CascadeText->Content("Phaser Cascade: ONLINE");
			else
				player2CascadeText->Content("Phaser Cascade: OFFLINE");
		}


		if (player3CascadeText != nullptr)
		{
			if (PlayerCascades[2])
				player3CascadeText->Content("Phaser Cascade: ONLINE");
			else
				player3CascadeText->Content("Phaser Cascade: OFFLINE");
		}

		if (player4CascadeText != nullptr)
		{
			if (PlayerCascades[3])
				player4CascadeText->Content("Phaser Cascade: ONLINE");
			else
				player4CascadeText->Content("Phaser Cascade: OFFLINE");
		}

		if (difficultyText != nullptr)
		{
			auto difficulty = Game.GetDifficulty();
			std::string difficultyString = "";
			switch (difficulty)
			{
			case GlassEngine::Easy:
				difficultyString = "Difficulty: Easy";
				break;
			case GlassEngine::Normal:
				difficultyString = "Difficulty: Normal";
				break;
			case GlassEngine::Hard:
				difficultyString = "Difficulty: Hard";
				break;
			case GlassEngine::Insane:
				difficultyString = "Difficulty: Insance";
				break;
			default:
				break;
			}
			difficultyText->Content(difficultyString);
		}

		for (auto ui : uiObjects)
		{
			ui->Update();
			if (std::dynamic_pointer_cast<UISprite, UIObject>(ui))
			{
				auto sprite = std::dynamic_pointer_cast<UISprite, UIObject>(ui);
				Renderer.RenderUI(sprite->GetUISprite(), sprite->Position());
			}
			else if (std::dynamic_pointer_cast<Button, UIObject>(ui))
			{
				auto button = std::dynamic_pointer_cast<Button, UIObject>(ui);
				Renderer.RenderUI(button->GetUISprite(), button->Position());
				Renderer.RenderText(button->Position() + button->GetText()->Offest(), button->GetText());
			}
		}
		for (auto ui : uiObjects)
		{
			if (std::dynamic_pointer_cast<Text, UIObject>(ui))
			{
				Renderer.RenderText(ui->Position(), std::dynamic_pointer_cast<Text, UIObject>(ui));
			}
		}
		Renderer.AddDirtyRectangle(DirtyRectangle(Vec2i((int)0, (int)950), Vec2i(Renderer.GetScreenDimentions().width, 130)));
		if (loadNewLevel)
			Game.LoadNewLevel("Resources/Levels/Demo Level 1.xml");
	}

	void UIManager::Stop()
	{
		uiObjects.clear();
		delete instance;
	}

	void UIManager::Reset()
	{
		uiObjectsBuffer = uiObjects;
		scoreText == nullptr;
		killCountText = nullptr;
		difficultyText = nullptr;
		for (auto uiObject : uiObjects)
			uiObject.reset();

		uiObjects.clear();
	}

	void UIManager::ConfirmReset()
	{
		for (auto uiObject : uiObjectsBuffer)
			uiObject.reset();

		uiObjectsBuffer.clear();
		scoreText.reset();
		killCountText.reset();
	}

	void UIManager::RevertReset()
	{
		uiObjects = uiObjectsBuffer;

		for (auto uiObject : uiObjectsBuffer)
			uiObject.reset();

		uiObjectsBuffer.clear();
	}
}