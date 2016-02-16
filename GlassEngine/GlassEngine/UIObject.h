#if !defined (UIOBJECT)
#define UIOBJECT

#include "precomp.h"

namespace GlassEngine{

	enum uiObjectType
	{
		UI_Text,
		UI_Sprite,
		UI_Button
	};

	class UIObject
	{
	public:  
		UIObject(){};
		virtual ~UIObject(){};

		std::vector<SmartPtr<UIObject>> GetChildren() const { return childUIObjects; };
		void AddUIObject(SmartPtr<UIObject> newUIObject){ childUIObjects.push_back(newUIObject); };
		void SetObjectType(uiObjectType newType){ type = newType; };
		
		uiObjectType GetObjectType() const { return type; };
		
		Vec2d Position() const { return position; };
		void Position(const Vec2d newPosition){ position = newPosition; };

	protected:
		std::vector<SmartPtr<UIObject>> childUIObjects;
		Vec2d position;
		uiObjectType type;
	};

}
#endif