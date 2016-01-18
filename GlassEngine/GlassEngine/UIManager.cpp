#include "precomp.h"
#include "UIManager.h"


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
		delete instance;
	}
	
	void UIManager::Update()
	{
	}

	void UIManager::Stop()
	{
	}
}