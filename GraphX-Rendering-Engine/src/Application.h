#pragma once

namespace engine
{
	class Window;
	class Event;

	class Application
	{
	private:
		/* Application window */
		Window* m_Window;

		/* Title of the application window */
		std::string m_Title;

	private:
		/* Callback for events */
		void OnEvent(Event& e);

	public:
		Application(std::string& title, int width, int height);

		/* Initializes the OpenGL elements, Returns whether initialization was successful */
		bool InitializeOpenGL();

		/* Application main method */
		void Run();

		~Application();
	};
}