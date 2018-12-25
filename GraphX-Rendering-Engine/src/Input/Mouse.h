#pragma once

namespace engine
{
	enum MouseButton
	{
		GX_MOUSE_LEFT = 0,
		GX_MOUSE_RIGHT = 1,
		GX_MOUSE_MIDDLE = 2
	};

	/* */
	class Mouse
	{
	private:
		/* Whether the left mouse button is pressed or not */
		bool m_LeftButtonPressed : 1;

		/* Whether the right mouse button is pressed or not */
		bool m_RightButtonPressed : 1;

		/* Whether the middle mouse button is pressed or not */
		bool m_MiddleButtonPressed : 1;

		/* Senstivity of the mouse i.e. how much the in application units will be changed with a unit change in device position */
		float m_Senstivity;

		/* Position of the mouse */
		gm::Vector2 m_Position;

		/* Static instance of the Mouse */
		static std::shared_ptr<Mouse> s_Mouse;

		/* To use as a constructor argument to make it private */
		struct ConstructorHelper{};

	public:
		/* Mouse Constructor is private (Constructor Helper is private) */
		Mouse(ConstructorHelper&& ch);

		/* Initialise the mouse */
		static void Init();

		/* Returns the static instance of the mouse */
		static std::shared_ptr<Mouse>& GetMouse() { return s_Mouse; }

		/* Mouse Event handlers */
		void OnEvent(class MouseButtonPressedEvent& e);
		
		void OnEvent(class MouseButtonReleasedEvent& e);
		
		void OnEvent(class MouseMovedEvent& e);

		/* Returns the mouse position */
		inline const gm::Vector2& GetPosition() const { return m_Position; }

		/* Returns if the left mouse button is pressed */
		inline bool IsLeftButtonPressed() const { return m_LeftButtonPressed; }

		/* Returns if the right mouse button is pressed */
		inline bool IsRightButtonPressed() const { return m_RightButtonPressed; }

		/* Returns if the middle mouse button is pressed */
		inline bool IsMiddleButtonPressed() const { return m_MiddleButtonPressed; }
	};
}