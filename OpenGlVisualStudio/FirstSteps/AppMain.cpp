/******************************************************************************************************************************************
***															INCLUDE FILES																***
*******************************************************************************************************************************************/
#include <iostream>
#include <string>

/*GLAD has to be included first!*/
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "stb_image.h"

#include "DisplayManager.h"
//#include "ShaderHandler.h"
#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


/******************************************************************************************************************************************
***														DEFINITIONS	AND TYPEDEFS														***
*******************************************************************************************************************************************/
#define MAIN_DISPLAY_WIDTH 1024
#define MAIN_DISPLAY_HEIGHT 768

#define SENSITIVITY_TRANSFORM (float)0.05f
#define SENSITIVITY_ROTATE (float)0.05f
#define SENSITIVITY_SCALE (float)0.05f

/******************************************************************************************************************************************
***															DATA AND PROTOTYPES															***
*******************************************************************************************************************************************/
/*Callback to adjust rendering window if the display window sizes were modified*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, glm::mat4* arg_modelMatrix_ptr, glm::mat4* arg_viewMatrix_ptr);
void printRenderTime();

float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
//unsigned int indices[] = {
//	0, 1, 3, // first triangle
//	1, 2, 3  // second triangle
//};

// For speed computation
double lastTime = glfwGetTime();
int nbFrames = 0;


/******************************************************************************************************************************************
***																MAIN																	***
*******************************************************************************************************************************************/
int main()
{
	/*Create window object*/
	GLFWwindow* AppMain_window = DispMngr_CreateDisplay(MAIN_DISPLAY_WIDTH, MAIN_DISPLAY_HEIGHT, "OpenGL Window");
	if (AppMain_window == NULL)
	{
		std::cout << "[ERROR] >>> Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(AppMain_window);

	/*Initialize GLAD to get OS specific function pointers*/
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "[ERROR] >>> Failed to initialize GLAD" << std::endl;
		return -1;
	}

	/*Specify rendering window*/
	glViewport(0, 0, MAIN_DISPLAY_WIDTH, MAIN_DISPLAY_HEIGHT);

	/*configure global opengl state*/
	glEnable(GL_DEPTH_TEST);

	/*Register the callback function for  render window resizing*/
	glfwSetFramebufferSizeCallback(AppMain_window, framebuffer_size_callback);


	/**********		Build and compile our shader program		***********/
	Shader ourShader("VertexShader.glsl", "FragmentShader.glsl");

	unsigned int VBO, VAO;// , EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	/**********			Binding shader program			***********/
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);
	// 0. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// load and create a texture 
	// -------------------------
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	unsigned char *data = stbi_load("../res/texture_rock.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "[ERROR] >>> Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	//glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	//glm::mat4 trans = glm::mat4(1.0f);

	unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
	glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	
	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	ourShader.use();
	ourShader.setInt("texture1", 0);


	
	// create transformations
	glm::mat4 modelMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	glm::mat4 viewMatrix = glm::mat4(1.0f);
	glm::mat4 projectionMatrix = glm::mat4(1.0f);

	viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, -3.0f)); // move back the opbject a little in space

	/***************			Render loop				********************/
	while (!glfwWindowShouldClose(AppMain_window))
	{
		/********			Measure Speed		***********/
		printRenderTime();

		/********			Handle inputs		***********/
		processInput(AppMain_window, &modelMatrix, &viewMatrix);


		/********		Rendering commands		***********/
		/* Clear color bit from previous iteration*/
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/* Bind Texture */
		glBindTexture(GL_TEXTURE_2D, texture);

		ourShader.use();

		/*Render image*/	
		projectionMatrix = glm::perspective(glm::radians(45.0f), (float)MAIN_DISPLAY_WIDTH / (float)MAIN_DISPLAY_HEIGHT, 0.1f, 100.0f);
		// retrieve the matrix uniform locations
		unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
		unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");
		// pass them to the shaders (3 different ways)
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &viewMatrix[0][0]);
		// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
		ourShader.setMat4("projection", projectionMatrix);

		// render box
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		/*Swap color buffers that has been used to draw in during this iteration and show it as output to the screen*/
		glfwSwapBuffers(AppMain_window);
		/*Check for events (user inputs, etc. Callbacks needs to be assigned!)*/
		glfwPollEvents();
	}

	/*Delete allocated resources*/
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);

	glfwTerminate();

	return 0;
}


/******************************************************************************************************************************************
***															LOCAL FUNCTIONS																***
*******************************************************************************************************************************************/
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window,  glm::mat4* arg_modelMatrix_ptr, glm::mat4* arg_viewMatrix_ptr)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){glfwSetWindowShouldClose(window, true);}
	if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS){*arg_viewMatrix_ptr = glm::translate(*arg_viewMatrix_ptr, glm::vec3(0.0f, 0.0f, -SENSITIVITY_SCALE));}
	if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS){*arg_viewMatrix_ptr = glm::translate(*arg_viewMatrix_ptr, glm::vec3(0.0f, 0.0f, SENSITIVITY_SCALE));}
	if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS){*arg_viewMatrix_ptr = glm::translate(*arg_viewMatrix_ptr, glm::vec3(-SENSITIVITY_TRANSFORM, 0.0f, 0.0f));}
	if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS){*arg_viewMatrix_ptr = glm::translate(*arg_viewMatrix_ptr, glm::vec3(SENSITIVITY_TRANSFORM, 0.0f, 0.0f));}
	if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS){*arg_viewMatrix_ptr = glm::translate(*arg_viewMatrix_ptr, glm::vec3(0.0f, SENSITIVITY_TRANSFORM, 0.0f));}
	if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS){*arg_viewMatrix_ptr = glm::translate(*arg_viewMatrix_ptr, glm::vec3(0.0f, -SENSITIVITY_TRANSFORM, 0.0f));}
	if (glfwGetKey(window, GLFW_KEY_KP_MULTIPLY) == GLFW_PRESS) { *arg_modelMatrix_ptr = glm::rotate(*arg_modelMatrix_ptr, SENSITIVITY_ROTATE, glm::vec3(0.0f, 1.0f, 0.0f));}
	if (glfwGetKey(window, GLFW_KEY_KP_DIVIDE) == GLFW_PRESS) { *arg_modelMatrix_ptr = glm::rotate(*arg_modelMatrix_ptr, SENSITIVITY_ROTATE, glm::vec3(1.0f, 0.0f, 0.0f));}

	if (glfwGetKey(window, GLFW_KEY_KP_0) == GLFW_PRESS) 
	{ 
		*arg_modelMatrix_ptr = glm::mat4(1.0f);
		*arg_viewMatrix_ptr = glm::mat4(1.0f);
		*arg_viewMatrix_ptr = glm::translate(*arg_viewMatrix_ptr, glm::vec3(0.0f, 0.0f, -3.0f));
	}
}

void printRenderTime()
{
	double currentTime = glfwGetTime();
	nbFrames++;
	if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1sec ago
		// printf and reset
		printf("[DIAG] >>> %f ms/frame\n", 1000.0 / double(nbFrames));
		nbFrames = 0;
		lastTime += 1.0;
	}
}