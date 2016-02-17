#include "precomp.h"
#include "UIObject.h"

namespace GlassEngine
{
	void UIObject::AddUIObject(SmartPtr<UIObject> newUIObject)
	{ 
		childUIObjects.push_back(newUIObject); 
	}

	std::vector<SmartPtr<UIObject>> UIObject::GetChildren() const
	{
		return childUIObjects;
	}

	Vec2d UIObject::Position() const 
	{ 
		return position; 
	};
	void UIObject::Position(const Vec2d newPosition)
	{ 
		position = newPosition; 
	};

	Vec2d UIObject::Offest() const 
	{ 
		return offset; 
	};

	void UIObject::Offest(const Vec2d newOffset)
	{ 
		offset = newOffset; 
	};
}