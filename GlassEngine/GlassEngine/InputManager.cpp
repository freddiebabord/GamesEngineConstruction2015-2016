#include "precomp.h"
#include "InputManager.h"

namespace GlassEngine{

	InputManager *InputManager::instance{ nullptr };

	InputManager::~InputManager()
	{ 
		
	};
	InputManager &InputManager::Instance()
	{
		if (instance == nullptr)
			instance = new InputManager;
		return *instance;
	}

	bool InputManager::ControllerIDExists(int id)
	{
		if (activeControllersThisFrame[id])
			return true;
		else
			return false;
	}

	bool InputManager::WasControllerDisconnectedLastUpdate(int id)
	{
		if (activeControllersLastFrame[id] && !activeControllersThisFrame[id])
			return true;
		else
			return false;
	}

	bool InputManager::WasControllerConnectedLastUpdadate(int id)
	{
		if (!activeControllersLastFrame[id] && activeControllersThisFrame[id])
			return true;
		else
			return false;
	}
	void InputManager::Reset()
	{
		for (int i = 0; i < maxControllers; ++i)
		{
			activeControllersThisFrame[i] = false;
			activeControllersLastFrame[i] = false;
		}
	}

	void InputManager::Start()
	{
		HAPI->GetKeyboardData(&Input.keyboard);
		HAPI->GetKeyboardData(&Input.keyboardBuffer);
		HAPI->GetMouseData(&Input.mouse);
		activeControllersThisFrame = new bool[maxControllers];
		activeControllersLastFrame = new bool[maxControllers];
		controllers = new HAPI_TControllerData[maxControllers];
		for (int i = 0; i < maxControllers; ++i)
		{
			activeControllersThisFrame[i] = false;
			activeControllersLastFrame[i] = false;
		}
		time = HAPI->GetTime();
	}

	void InputManager::Update()
	{
		if (time < HAPI->GetTime() - (1000 / 60))
		{			//Update the current time
			time = HAPI->GetTime();
			Input.keyboardBuffer = Input.keyboard;
			Input.controllersBuffer = *Input.controllers;
			HAPI->GetKeyboardData(&Input.keyboard);
			HAPI->GetMouseData(&Input.mouse);
			for (int i = 0; i < 8; i++)
			{
				activeControllersLastFrame[i] = activeControllersThisFrame[i];
			}
			
			for (int i = 0; i < maxControllers; ++i)
			{
				if (HAPI->GetControllerData(i, &Input.controllers[i]))
				{
					activeControllersThisFrame[i] = true;
				}
				else
				{
					activeControllersThisFrame[i] = false;
				}
			}
		}
	}

	// TODO: Delete controller buffer somehow...
	void InputManager::Stop()
	{
		delete[] activeControllersLastFrame;
		delete[] activeControllersThisFrame;
		//delete controllersBuffer;
		delete controllers;
		//delete controllersBuffer;
		delete instance;
	}

}