#pragma once
/*Include files*/
#include <iostream>
/*GLAD has to be included first!*/
#include <glad\glad.h>
#include <GLFW\glfw3.h>


GLFWwindow* DispMngr_CreateDisplay(int arg_width, int arg_height, const char* title);
