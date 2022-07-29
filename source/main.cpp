#include <iostream>

#include "App.h"

void PauseApp()
{
	char x = ' ';
	std::cin >> x;
}

int main()
{
	// Read app config
	AppConfig config = { 1080, 720, "#version 330" };

	// Initialize glfw
	if (!glfwInit())
	{
		std::cout << "ERROR: Failed to initialize glfw\n";
		PauseApp();

		return 0;
	}

	// Create the native window handle and create the OpenGL context
	GLFWwindow* windowHandle = glfwCreateWindow(config.width, config.height, "Group Project", nullptr, nullptr);
	glfwMakeContextCurrent(windowHandle);

	// Initialize glew (OpenGL function call wrangler library)
	if (glewInit() != GLEW_OK)
	{
		std::cout << "ERROR: Failed to initialize glew wrangling library\n";
		PauseApp();

		return 0;
	}

	// Create the app
	App app(windowHandle, config);
	app.Initialize();

	while (!glfwWindowShouldClose(windowHandle))
	{
		glfwPollEvents();

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		app.Update();

		glfwSwapBuffers(windowHandle);
	}

	glfwDestroyWindow(windowHandle);
	glfwTerminate();
	
	return 0;
}