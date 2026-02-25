#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main()
{
	// Check to see if glfw lib is initialised
	if (!glfwInit())
	{
		return -1;
	}

	// Set up context to associate to window created below
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Standard for creating window in glfw
	GLFWwindow* window = glfwCreateWindow(1280, 720, "OpenGLProject", nullptr, nullptr);

	// If error creating window, terminate
	if (window == nullptr) 
	{
		std::cout << "Error creating window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Where the window will open on local screen space
	glfwSetWindowPos(window, 500, 300);

	// Assign our window as the current context
	glfwMakeContextCurrent(window);

	// Initialise glew
	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		return -1;
	}

	// Main event loop, continues until window is closed
	while (!glfwWindowShouldClose(window))
	{
		// Creates a background colour
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Need to call so that back and front buffer are swapped so we can see the things we want rendered
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// End glfw processes
	glfwTerminate();

	return 0;
}
