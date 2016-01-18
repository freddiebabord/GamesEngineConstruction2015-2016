#if !defined (INPUT_MANAGER)
#define INPUT_MANAGER

#include "Vector2.h"
#include "Manager.h"

namespace GlassEngine{

#define Input InputManager::Instance()

	class InputManager : public Manager
	{
	public:
		static InputManager &Instance();
		~InputManager();

		void Start() override;
		void Update() override;
		void Stop() override;

		template <typename T>
		bool GetKey(T key){
			return Input.keyboard.scanCode[key];
		};

		template <typename T>
		bool GetKeyDown(T key)
		{
			if (Input.keyboard.scanCode[key] && !Input.keyboardBuffer.scanCode[key])
				return true;
			else
				return false;
		};

		template <typename T>
		bool GetKeyUp(T key)
		{
			if (!Input.keyboard.scanCode[key] && Input.keyboardBuffer.scanCode[key])
				return true;
			else
				return false;
		};

		template <typename T>
		bool GetButton(T button, int controllerIndex)
		{
			return Input.controllers[controllerIndex].digitalButtons[button];
		};

		template <typename T>
		int GetAxis(T axis, int controllerIndex)
		{
			return Input.controllers[controllerIndex].analogueButtons[axis];
		};

		bool ControllerIDExists(int id);
		bool WasControllerDisconnectedLastUpdate(int id);
		bool WasControllerConnectedLastUpdadate(int id);
		void Reset();
		const Vec2i GetMousePosition(){ return Vec2i(mouse.x, mouse.y); };
		const int GetMiddleMouseMovement() { return mouse.wheelMovement; };
		const bool GetLeftMouseButton() { return mouse.leftButtonDown; };
		const bool GetRightMouseButton() { return mouse.rightButtonDown; };
	protected:
		InputManager(){};

	private:
		static InputManager *instance;
		static const int maxControllers = 4;
		DWORD time;

		HAPI_TKeyboardData keyboard;
		HAPI_TKeyboardData keyboardBuffer;
		HAPI_TMouseData mouse;
		HAPI_TControllerData *controllers;
		bool* activeControllersThisFrame;
		bool* activeControllersLastFrame;
	};

}

#endif //INPUT_MANAGER