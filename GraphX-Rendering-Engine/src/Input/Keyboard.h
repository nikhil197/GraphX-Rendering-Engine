#pragma once

namespace engine
{
	/* Keycodes accepted by the GraphX engine */
	enum Keys
	{
		GX_UNKNOWN = -1,
		GX_KP_0 = 0,
		GX_KP_1,
		GX_KP_2,
		GX_KP_3,
		GX_KP_4,
		GX_KP_5,
		GX_KP_6,
		GX_KP_7,
		GX_KP_8,
		GX_KP_9,
		GX_KP_DECIMAL,
		GX_KP_DIVIDE,
		GX_KP_MULTIPLY,
		GX_KP_SUBTRACT,
		GX_KP_ADD,
		GX_KP_ENTER,
		GX_KP_EQUAL,
		GX_LEFT_SHIFT = 20,
		GX_LEFT_CONTROL,
		GX_LEFT_ALT,
		GX_LEFT_SUPER,
		GX_RIGHT_SHIFT,
		GX_RIGHT_CONTROL,
		GX_RIGHT_ALT,
		GX_RIGHT_SUPER,
		GX_MENU,
		GX_LAST = GX_MENU,

		GX_SPACE = 32,
		GX_APOSTROPHE = 39,
		GX_COMMA = 44,
		GX_MINUS,
		GX_PERIOD,
		GX_SLASH,
		GX_0,
		GX_1,
		GX_2,
		GX_3,
		GX_4,
		GX_5,
		GX_6,
		GX_7,
		GX_8,
		GX_9,
		GX_SEMICOLON = 59,
		GX_EQUAL = 61,
		GX_A = 65,
		GX_B,
		GX_C,
		GX_D,
		GX_E,
		GX_F,
		GX_G,
		GX_H,
		GX_I,
		GX_J,
		GX_K,
		GX_L,
		GX_M,
		GX_N,
		GX_O,
		GX_P,
		GX_Q,
		GX_R,
		GX_S,
		GX_T,
		GX_U,
		GX_V,
		GX_W,
		GX_X,
		GX_Y,
		GX_Z,
		GX_LEFT_BRACKET,
		GX_BACKSLASH,
		GX_RIGHT_BRACKET,
		GX_GRAVE_ACCENT = 96,
		
		GX_ESCAPE = 106,
		GX_ENTER,
		GX_TAB,
		GX_BACKSPACE,
		GX_INSERT,
		GX_DELETE,
		GX_RIGHT,
		GX_LEFT,
		GX_DOWN,
		GX_UP,
		GX_PAGE_UP,
		GX_PAGE_DOWN,
		GX_HOME,
		GX_END,
		GX_CAPS_LOCK = 130,
		GX_SCROLL_LOCK,
		GX_NUM_LOCK,
		GX_PRINT_SCREEN,
		GX_PAUSE,
		GX_F1 = 140,
		GX_F2,
		GX_F3,
		GX_F4,
		GX_F5,
		GX_F6,
		GX_F7,
		GX_F8,
		GX_F9,
		GX_F10,
		GX_F11,
		GX_F12
	};

	class Keyboard
	{
	private:
		/* To keep track of which keys have been pressed */
		std::vector<bool>* m_Keys;

		/* Static instance of the keyboard */
		static std::shared_ptr<Keyboard> s_Keyboard;

		/* To use as a constructor argument to make it private */
		struct ConstructorHelper{};

	public:
		/* Constructor helper is private to this class only, so the constructor cannot be invoked */
		Keyboard(ConstructorHelper&& ch);
		
		/* Initialize the keyboard */
		static void Init();

		/* Returns the static instance of the keyboard */
		static const std::shared_ptr<Keyboard>& GetKeyboard() { return s_Keyboard; }

		/* Key event handlers */
		void OnEvent(class KeyPressedEvent& e);

		void OnEvent(class KeyReleasedEvent& e);

		/* Returns whether the specified key is pressed or not */
		inline bool GetKey(int keyCode) const 
		{
			if (keyCode < 0)
				return false;
			return (*m_Keys)[keyCode];
		}

		~Keyboard();
	};
}