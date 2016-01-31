#include "Dependencies\glew.h"
#include "Dependencies\glfw3.h"
#include <cstdio>
#include <iostream>

#include "game.h"

GLFWwindow* window;


void initFunc() {
	gameInit();
}

void displayFunc() {
	gameUpdate();
}

void endFunc() {
	gameEnd();
}

void resizeFunc(GLFWwindow* window, int w, int h) {
	gameResize(w, h);
	gameUpdate();
	glfwSwapBuffers(window);
}

bool keys[512];

void keyFunc(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (action == GLFW_PRESS) {
		keys[key] = true;
	} else if (action == GLFW_RELEASE) {
		keys[key] = false;
	}
}

void scrollFunc(GLFWwindow* window, double x, double y) {
	using namespace std;
	cout << "x = " << x << endl << "y = " << y << endl;

	changeVal(y);
}

void mouseFunc(GLFWwindow* window, double x, double y) {
	gameMouse(x, y);
}

int main(int argc, char** argv) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	bool fullscreen = false;
	window = glfwCreateWindow(1280, 720, "Voxel Slice Rendering", fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);
	// get the current Desktop screen resolution and colour depth

	if (window == nullptr) {
		printf("Failed to create window\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		printf("Failed to init glew\n");
		return -1;
	}

	initFunc();
	glfwSwapInterval(1);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetWindowSizeCallback(window, resizeFunc);
	glfwSetKeyCallback(window, keyFunc);
	glfwSetScrollCallback(window, scrollFunc);
	glfwSetCursorPosCallback(window, mouseFunc);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		displayFunc();
		glfwSwapBuffers(window);
	}

	endFunc();
	glfwTerminate();
	return 0;
}

