#include "precomp.h"
#include "UIManager.h"
#include "Text.h"
#include "GameObject.h"
#include "Transform.h"
namespace GlassEngine
{
	UIManager *UIManager::instance{ nullptr };

	UIManager &UIManager::Instance()
	{
		if (instance == nullptr)
			instance = new UIManager;
		return *instance;
	}

	void UIManager::Start()
	{

	}
	
	void UIManager::Update()
	{
		
		for (const auto t : uiObjects)
		{
			auto p = t->GetParent()->GetTransform()->GetPosition();
			HAPI->ChangeFont(font, size);
			HAPI->RenderText((int)p.x, (int)p.y, HAPI_TColour(255, 0, 0, 255), t->GetContent());
		}
	}

	void UIManager::Stop()
	{
		uiObjects.clear();
		delete instance;
	}
}