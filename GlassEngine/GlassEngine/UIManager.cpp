#include "precomp.h"
#include "UIManager.h"
#include "Text.h"
#include "GameObject.h"
#include "Transform.h"
#include "RenderManager.h"
#include "Button.h"

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
		
		/*for (const auto t : uiObjects)
		{
			auto p = t->GetParent()->GetTransform()->GetPosition();
			HAPI->ChangeFont(font, size);
			HAPI->RenderText((int)p.x, (int)p.y, HAPI_TColour(255, 0, 0, 255), t->Content());
		}*/
		for (auto ui : uiObjects)
		{
			
			if (ui->GetObjectType() == uiObjectType::UI_Sprite)
			{

			}
			else if (ui->GetObjectType() == uiObjectType::UI_Button)
			{
				auto button = std::dynamic_pointer_cast<Button, UIObject>(ui);
				Renderer.RenderUI(button->GetUISprite(), button->Position());
				Renderer.RenderText(button->Position(), button->GetText());
			}
			
		}
		for (auto ui : uiObjects)
		{
			if (ui->GetObjectType() == uiObjectType::UI_Text)
			{
				Renderer.RenderText(ui->Position(), std::dynamic_pointer_cast<Text, UIObject>(ui));
			}
		}
		Renderer.AddDirtyRectangle(DirtyRectangle(Vec2i((int)0, (int)950), Vec2i(Renderer.GetScreenDimentions().width, 130)));
	}

	void UIManager::Stop()
	{
		uiObjects.clear();
		delete instance;
	}
}