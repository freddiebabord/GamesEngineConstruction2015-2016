#pragma once
#include "GameObject.h"
#include "UIObject.h"

namespace GlassEngine{

	class Sprite;

	enum ButtonStates
	{
		normal,
		hover,
		disabled,
		click
	};

	class Button : public UIObject, public GameObject
	{
	public:
		Button();
		~Button();

		virtual void Start() override;
		virtual void Update() override;
		virtual void Stop() override;

		std::shared_ptr<Sprite> normalSprite;
		std::shared_ptr<Sprite> hoverSprite;
		std::shared_ptr<Sprite> disabledSprite;
		std::shared_ptr<Sprite> clickSprite;

	private:
		ButtonStates state = ButtonStates::normal;
	};


}