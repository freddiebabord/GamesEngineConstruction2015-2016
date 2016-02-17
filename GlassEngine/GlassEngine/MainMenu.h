#if !defined (MAINMENU)
#define MAINMENU

#include "Level.h"
#include "Button.h"
#include "Game.h"

using namespace GlassEngine;

class MainMenu : public Level
{
public:
	MainMenu() : Level(){};
	MainMenu(int previousScore_) : Level(){ previousScore = previousScore_; };
	virtual ~MainMenu(){};

	void OnStart() override;
	void Update() override;

private:
	int previousScore = 0;
	SmartPtr<Text> scoreText;
	SmartPtr<Text> StartText;
};


class StartGameButton : public Button
{
public:
	StartGameButton() { SetUISprite(0); };

	void Update() override;

	void OnHover() override;
	void OnClick() override;
};


#endif