#pragma once
#include "Manager.h"

namespace GlassEngine{

#define UI UIManager::Instance()

	class UIObject;

	class UIManager : public Manager
	{
	public:
		static UIManager &Instance();
		~UIManager(){};

		void Start() override;
		void Update() override;
		void Stop() override;

		void AddUIElement(std::shared_ptr<UIObject> obj){ uiObjects.push_back(obj); };
		std::vector<std::shared_ptr<UIObject>>& GetUIObjects() { return uiObjects; };

	private:
		UIManager(){};
		static UIManager *instance;
		std::vector<std::shared_ptr<UIObject>> uiObjects;
	};

}