#if !defined (UISPRITE)
#define UISPRITE

#include "UIObject.h"

namespace GlassEngine{

class UISprite : public UIObject
{
public:
	UISprite() : UIObject(){};
	virtual ~UISprite(){};

	void SetUISprite(const int newUISpriteRefreance){ uiSpriteID = newUISpriteRefreance; };
	int GetUISprite() const { return uiSpriteID; };

protected:
	int uiSpriteID = -1;
};

}
#endif