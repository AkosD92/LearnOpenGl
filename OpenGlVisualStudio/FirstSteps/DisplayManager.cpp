#include "DisplayManager.h"

GLFWwindow* DispMngr_CreateDisplay(int arg_width, int arg_height, const char* title)
{
	/*Initilaize GLFW window*/
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment for mac


	return glfwCreateWindow(arg_width, arg_height, title, NULL, NULL);

}