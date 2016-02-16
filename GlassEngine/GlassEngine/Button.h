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
	
		void SetText(SmartPtr<Text> newText){ text = newText; };
		SmartPtr<Text> GetText() { return text; };
	
		void SetUISprite(const int newUISpriteRefreance){ uiSpriteID = newUISpriteRefreance; };
		int GetUISprite() const { return uiSpriteID; };

	private:
		int uiSpriteID = -1;
		SmartPtr<Text> text;
	};
}

#endif