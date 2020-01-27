#pragma once

namespace GraphX
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
		/* To use as a constructor argument to make it private */
		struct ConstructorHelper{};

	public:
		/* Mouse Constructor is private (Constructor Helper is private) */
		Mouse(ConstructorHelper&& ch);

		/* Sensitivity of the mouse i.e. how much the pointer position in application units will be changed with a unit change in device position */
		float Sensitivity;

		/* Sensitivity of the mouse scroll i.e. how much the scroll in application units will be changed with a unit change in device scroll */
		float ScrollSenstivity;

		/* Initialise the mouse */
		static void Init();

		/* Returns the static instance of the mouse */
		static const std::shared_ptr<Mouse>& GetMouse() { return s_Mouse; }

		/* Mouse Event handlers */
		void OnEvent(class MouseButtonPressedEvent& e);
		
		void OnEvent(class MouseButtonReleasedEvent& e);
		
		void OnEvent(class MouseMovedEvent& e);

		void OnEvent(class MouseScrolledEvent& e);

		/* Update the mouse */
		void Update();

		/* Returns the mouse position */
		inline const GM::Vector2& GetPosition() const { return m_Position; }

		/* Returns the last frame mouse position */
		inline const GM::Vector2& GetLastPosition() const { return m_LastPosition; }

		/* Returns the current scroll offset */
		inline const GM::Vector2& GetScrollOffset() const { return m_ScrollOffset; }

		/* Returns if the left mouse button is pressed */
		inline bool IsLeftButtonPressed() const { return m_LeftButtonPressed; }

		/* Returns if the right mouse button is pressed */
		inline bool IsRightButtonPressed() const { return m_RightButtonPressed; }

		/* Returns if the middle mouse button is pressed */
		inline bool IsMiddleButtonPressed() const { return m_MiddleButtonPressed; }

	private:
		/* Whether the left mouse button is pressed or not */
		bool m_LeftButtonPressed : 1;

		/* Whether the right mouse button is pressed or not */
		bool m_RightButtonPressed : 1;

		/* Whether the middle mouse button is pressed or not */
		bool m_MiddleButtonPressed : 1;

		/* Position of the mouse */
		GM::Vector2 m_Position;

		/* Position of the mouse in the last frame */
		GM::Vector2 m_LastPosition;

		/* Current frame offset of the mouse scroll */
		GM::Vector2 m_ScrollOffset;

		/* Static instance of the Mouse */
		static std::shared_ptr<Mouse> s_Mouse;
	};
}