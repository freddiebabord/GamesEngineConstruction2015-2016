#pragma once
#include "Manager.h"

namespace GlassEngine{

#define UI UIManager::Instance()

	class UIObject;
	class Text;

	class UIManager : public Manager
	{
	public:
		static UIManager &Instance();
		~UIManager(){};

		void Start() override;
		void Update() override;
		void Stop() override;

		void AddUIElement(SmartPtr<UIObject> obj){ uiObjects.push_back(obj); };
		std::vector<SmartPtr<UIObject>>& GetUIObjects() { return uiObjects; };
		void Reset();
		void RevertReset();
		void ConfirmReset();

		void SetFont(const std::string font_, int size_){ font = font_; size = size_; };

		bool loadNewLevel = false;

		std::vector<bool> PlayerCascades;

	private:
		UIManager(){};
		static UIManager *instance;
		std::vector<SmartPtr<UIObject>> uiObjects;
		std::vector<SmartPtr<UIObject>> uiObjectsBuffer;

		SmartPtr<Text> scoreText = nullptr;
		SmartPtr<Text> killCountText = nullptr;
		SmartPtr<Text> difficultyText = nullptr;
		SmartPtr<Text> player1CascadeText = nullptr;
		SmartPtr<Text> player2CascadeText = nullptr;
		SmartPtr<Text> player3CascadeText = nullptr;
		SmartPtr<Text> player4CascadeText = nullptr;


		std::string font = "Calibri";
		int size = 25;
	};

}