#if !defined (BUTTON)
#define BUTTON

#include "precomp.h"
#include "UIObject.h"

namespace GlassEngine{

	class Text;

	class Button : public UIObject
	{
	public:

		Button() : UIObject(){};
		virtual ~Button(){};

		virtual void Update() override;

		void SetText(SmartPtr<Text> newText){ text = newText; };
		SmartPtr<Text> GetText() { return text; };
	
		void SetUISprite(const int newUISpriteRefreance){ uiSpriteID = newUISpriteRefreance; };
		int GetUISprite() const { return uiSpriteID; };

		void CreateButtonCollider();

		virtual void OnHover(){};
		virtual void OnClick(){};

	protected:
		int uiSpriteID = -1;
		Rect buttonRectangle;
		bool hover = false;
		SmartPtr<Text> text;
	};
}

#endif