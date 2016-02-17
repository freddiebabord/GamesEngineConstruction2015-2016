#include "precomp.h"
#include "Button.h"
#include "RenderManager.h"
#include "InputManager.h"

namespace GlassEngine{

	void Button::Update()
	{
		if (buttonRectangle.InsideBounds(Input.GetMousePosition()))
			hover = true;
		else
			hover = false;

		if (hover)
		{
			OnHover();
			if (Input.GetLeftMouseButton())
				OnClick();
		}
	}

	void Button::CreateButtonCollider()
	{
		auto size = Renderer.GetUISprite(uiSpriteID);
		Rect newRect = Rect(
			(int)position.y - (int)(size.y / 2), 
			(int)position.y + (int)(size.y / 2), 
			(int)position.x - (int)(size.x / 2), 
			(int)position.x + (int)(size.x / 2)
		);
		buttonRectangle = newRect;
	}

}