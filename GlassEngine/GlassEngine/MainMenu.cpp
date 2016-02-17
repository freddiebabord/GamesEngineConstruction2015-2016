#include "precomp.h"
#include "MainMenu.h"
#include "Button.h"
#include "Text.h"
#include "UIObject.h"
#include "UIManager.h"
#include "RenderManager.h"
#include "Game.h"
#include "Sprite.h"


using namespace GlassEngine;

void MainMenu::OnStart()
{
	Renderer.AddSprite("Resources/menu_background.jpg");
	Renderer.RenderBackground(Background, Vec3d(0, 0, 0));
	SmartPtr<Sprite> sprite = MakeSmartPtr<Sprite>();
	sprite->LoadSprite("Resources/UI/Button.png");
	sprite->ID(0);
	Renderer.AddUISprite(sprite);
	SmartPtr<Sprite> spriteHover = MakeSmartPtr<Sprite>();
	spriteHover->LoadSprite("Resources/UI/ButtonHover.png");
	spriteHover->ID(1);
	Renderer.AddUISprite(spriteHover);
	auto startGameButton = MakeSmartPtr<StartGameButton>();
	SmartPtr<Text> text = MakeSmartPtr<Text>();

	text->Content("Start Game");
	Font textFont;
	textFont.antiAliasing = true;
	textFont.fontSize = 20;
	textFont.fontWeight = 600;
	textFont.fontColour = HAPI_TColour(0, 0, 0);
	text->SetFont(textFont);
	text->Offest(Vec2d(-60, -5));
	startGameButton->SetUISprite(0);
	startGameButton->SetText(text);
	startGameButton->Position(Vec2d(960, 530));
	
	startGameButton->SetUISprite(0);
	
	startGameButton->CreateButtonCollider();
	UI.AddUIElement(startGameButton);


	StartText = MakeSmartPtr<Text>();
	Font StartTexttextFont;
	StartTexttextFont.antiAliasing = true;
	StartTexttextFont.fontSize = 50;
	StartTexttextFont.fontWeight = 900;
	StartTexttextFont.fontColour = HAPI_TColour(255, 255, 255);
	StartText->Content("Click 'Start Game', Press Space or Press A to Start");
	StartText->SetFont(StartTexttextFont);
	StartText->Position(Vec2d(350,800));
	UI.AddUIElement(StartText);


	scoreText = MakeSmartPtr<Text>();

	scoreText->Content("Previous Game Score: " + std::to_string(previousScore));
	scoreText->SetFont(StartTexttextFont);
	scoreText->Position(Vec2d(700, 870));
	UI.AddUIElement(scoreText);

	id = 0;
}

void MainMenu::Update()
{
	/*Renderer.RenderText(scoreText->Position(), scoreText);
	Renderer.RenderText(StartText->Position(), StartText);
*/
	if (scoreText == nullptr)
		OnStart();
}


void StartGameButton::Update()
{
	Button::Update();

	if (!hover && uiSpriteID != 0)
		uiSpriteID = 0;
}

void StartGameButton::OnHover()
{
	uiSpriteID = 1;
}

void StartGameButton::OnClick()
{
	UI.loadNewLevel = true;
}