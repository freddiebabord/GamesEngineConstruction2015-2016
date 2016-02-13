#pragma once
#include "Manager.h"

namespace GlassEngine{

#define UI UIManager::Instance()

	class Text;

	class UIManager : public Manager
	{
	public:
		static UIManager &Instance();
		~UIManager(){};

		void Start() override;
		void Update() override;
		void Stop() override;

		void AddUIElement(SmartPtr<Text> obj){ uiObjects.push_back(obj); };
		std::vector<SmartPtr<Text>>& GetUIObjects() { return uiObjects; };
		void Reset(){ uiObjectsBuffer = uiObjects; uiObjects.clear(); };
		void RevertReset(){ uiObjects = uiObjectsBuffer;  uiObjectsBuffer.clear(); };
		void ConfirmReset(){ uiObjectsBuffer.clear(); };

		void SetFont(const std::string font_, int size_){ font = font_; size = size_; };

	private:
		UIManager(){};
		static UIManager *instance;
		std::vector<SmartPtr<Text>> uiObjects;
		std::vector<SmartPtr<Text>> uiObjectsBuffer;
		std::string font = "Calibri";
		int size = 25;
	};

}