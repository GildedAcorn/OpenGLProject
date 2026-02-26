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
		std::cout << "ERROR: GLFW NOT INITIALISED!" << std::endl;
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
		layout (location = 1) in vec3 color;

		out vec3 vColor;

		void main()
		{
			gl_Position = vec4(position.x, position.y, position.z, 1.0);
			vColor = color;
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
		in vec3 vColor;
		uniform vec4 uColor;	

		void main()
		{
			FragColor = vec4(vColor, 1.0) * uColor;
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
	// Vertex locations for TRIANGLE
	// First 3 = vertices location
	// Second 3 = Color attributes RGB
	/*std::vector<float> vertices =
	{
		0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,

	};*/
	// Vertex locations for SQUARE
	// First 4 = vertex locations
	// Second 3 = color attribute RGB
	std::vector<float> vertices =
	{
		// vertex 		  // RGB 
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
	};

	// Create a new vector to store the indices of the SQUARE vertices to allow reuse of information
	std::vector<unsigned int> indices =
	{
		// Order to draw triangles 
		0, 1, 2,
		0, 2, 3,
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

	// Create element buffer object to store indices 
	GLuint ebo;
	// Create 1 buffer to assign indices values to memory
	glGenBuffers(1, &ebo);
	// Assign buffer into memory
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	// Send data to GPU memory
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	// Clear buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Create vertex array object to store vertex bindings
	GLuint vao = 0;
	// Assign vertices 
	glGenVertexArrays(1, &vao);
	// Activate it by creating a buffer that tells opengl that we want to assign the current vertex array object to the vertex buffer object
	glBindVertexArray(vao);
	// Bind buffer (DO THIS BEFORE YOU PUT SOME KIND OF DATA IN TO AVOID STARING AT THE SAME THING FOR AGES!!!!)
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// Bind buffer for the ebo as well for complex shapes
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	// Specify how to interpret the data (index, num of values, normalization, stride (size of 1 (3 values of vertex location, 3 for RGB)
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 6 * sizeof(float), (void*)0);
	// Enable attribute array
	glEnableVertexAttribArray(0);
	// Enable VAO to take in RBG values in the vertices array
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	// Enable to array
	glEnableVertexAttribArray(1);
	// Unbind buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind vertex array
	glBindVertexArray(0);

	// Get the uniform
	GLint uColorloc = glGetUniformLocation(shaderProgram, "uColor");



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
		// Set the uniform color
		glUniform4f(uColorloc, 0.0f, 1.0f, 0.0f, 1.0f);
		// Bind vertex array
		glBindVertexArray(vao);
		// Draw the triangle
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Need to call so that back and front buffer are swapped so we can see the things we want rendered
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// ---=== CLEANUP ===---
	// End glfw processes
	glfwTerminate();

	return 0;
}
