#include "precomp.h"
#include "Button.h"
#include "InputManager.h"

namespace GlassEngine{

	Button::Button()
	{
	}


	Button::~Button()
	{
	}

	void Button::Start()
	{

	}

	void Button::Update()
	{
		if (rect.CheckState(Input.GetMousePosition()) && (Input.GetLeftMouseButton() || Input.GetButton(HK_DIGITAL_A, 0)))
		{
			if (state != ButtonStates::hover)
			{
				if (hoverSprite)
					currentSprite = hoverSprite;
				state = ButtonStates::hover;
			}
		}
		else if (rect.CheckState(Input.GetMousePosition()) && (!Input.GetLeftMouseButton() || !Input.GetButton(HK_DIGITAL_A, 0)))
		{
			if (state != ButtonStates::hover)
			{
				if (hoverSprite)
					currentSprite = hoverSprite;
				state = ButtonStates::hover;
			}
		}
		else if (state != ButtonStates::normal && state != ButtonStates::disabled)
		{
			if (normalSprite)
				currentSprite = normalSprite;
			state = ButtonStates::normal;
		}
	}

	void Button::Stop()
	{

	}
}