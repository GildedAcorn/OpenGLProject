#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <string>

int main()
{
	// ---=== INIT GLFW ===---
	// Check to see if glfw lib is initialised
	if (!glfwInit())
	{
		return -1;
	}

	// ---=== WINDOW ===---
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

	// ---=== VERTEX SHADER ===---
	// Define source code for vertex shader
	std::string vertexShaderSource = R"(
		#version 330 core
		layout (location = 0) in vec3 position;

		void main()
		{
			gl_Position = vec4(position.x, position.y, position.z, 1.0);
		}
	)";

	// Create vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Convert vertex shader source into a C string to plug into the shader
	const char* vertexShaderCString = vertexShaderSource.c_str();
	// Compile vertex shader source
	glShaderSource(vertexShader, 1, &vertexShaderCString, nullptr);
	// Compile vertex shader
	glCompileShader(vertexShader);

	// Check for vertex compilation errors
	GLint success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		// Create buffer to store error log
		char infoLog[512];
		// Get the error message then print to screen
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cerr << "ERROR:VERTEX_SHADER_COMPLIATION_FAILED: " << infoLog << std::endl;
	}

	// ---=== FRAGMENT SHADER ===---
	std::string fragmentShaderSource = R"(
		#version 330 core
		out vec4 FragColor;

		void main()
		{
			FragColor = vec4(1.0, 0.0, 0.0, 1.0);
		}
	)";

	// Create fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Create fragment shader source
	const char* fragmentShaderCString = fragmentShaderSource.c_str();
	// Compile fragment shader source
	glShaderSource(fragmentShader, 1, &fragmentShaderCString, nullptr);
	// Compile fragment shader
	glCompileShader(fragmentShader);

	// Check for fragment compilation errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		std::cerr << "ERROR:FRAGMENT_SHADER_COMPILATION_FAILED: " << infoLog << std::endl;
	}

	// ---=== COMBINING THE SHADERS ===---
	// Create shader program
	GLuint shaderProgram = glCreateProgram();
	// Attach vertex shader
	glAttachShader(shaderProgram, vertexShader);
	// Attach fragment shader
	glAttachShader(shaderProgram, fragmentShader);
	// Link all of the shaders in program
	glLinkProgram(shaderProgram);

	// Check for shader program errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cerr << "ERROR:SHADER_PROGRAM_LINKING_FAILED: " << infoLog << std::endl;
	}

	// Delete shaders once compiled
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// ---=== VERTICES FOR SHAPES ===---
	// Vertices locations for drawing a triangle
	std::vector<float> vertices =
	{
		0.0f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
	};

	// ---=== SEND DATA TO GPU ===---
	// Create vertex buffer object to assign memory for the vertices
	GLuint vbo = 0;
	// Upload triangle data into GPU memory using buffers
	glGenBuffers(1, &vbo);
	// Assign buffer into memory
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// Send data to GPU memory
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
	// Clear buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Create vertex array object to store vertex bindings
	GLuint vao = 0;
	// Assign vertices 
	glGenVertexArrays(1, &vao);
	// Activate it by creating a buffer that tells opengl that we want to assign the current vertex array object to the vertex buffer object
	glBindVertexArray(vao);
	// Bind buffer (DO THIS BEFORE YOU PUT SOME KIND OF DATA IN TO AVOID STARING AT THE SAME THING FOR AGES!!!!)
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// Specify how to interpret the data
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), (void*)0);
	// Enable attribute array
	glEnableVertexAttribArray(0);
	// Unbind buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind vertex array
	glBindVertexArray(0);



	// ---=== MAIN LOOP ===---
	// Main event loop, continues until window is closed
	while (!glfwWindowShouldClose(window))
	{
		// Creates a background colour
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// ---=== DRAW TRIANGLE ===---
		// Assign shader program to use
		glUseProgram(shaderProgram);
		// Bind vertex array
		glBindVertexArray(vao);
		// Draw the triangle
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Need to call so that back and front buffer are swapped so we can see the things we want rendered
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// ---=== CLEANUP ===---
	// End glfw processes
	glfwTerminate();

	return 0;
}
