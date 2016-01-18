#pragma once

namespace GlassEngine{

	class Sprite;

	class UIObject
	{
	public:
		UIObject();
		virtual ~UIObject();

		
		std::shared_ptr<Sprite> currentSprite;
		Rect rect;
		
	};

}