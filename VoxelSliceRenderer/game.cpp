#include "game.h"
#include "Dependencies\glew.h"
#include "Dependencies\glfw3.h"
#include <cstdio>
#include <iostream>
#include "ShaderProgram.h"
#include "map_data.h"
#include "Camera.h"
#include "Common\config.h"

ShaderProgram* shaders;
GLuint map_tex;
GLuint quad_vao;
double val = 2;
int width = 1280, height = 720;
Camera* cam;
Config config;
bool gravity;

const GLfloat quadVertices[] = {
	-1.0f,  1.0f,
	-1.0f, -1.0f,
	1.0f, -1.0f,

	-1.0f,  1.0f,
	1.0f, -1.0f,
	1.0f,  1.0f
};

void gameInit() {
	config.load("settings.conf");
	config.debug_print();
	val = config.get_float("val", 1.0f);
	gravity = config.get_bool("gravity", false);
	int size;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &size);
	printf("Max texture size = %i\n", size);

	shaders = new ShaderProgram("Shaders\\shader.vert", "Shaders\\shader.frag");

	glGenTextures(1, &map_tex);
	glBindTexture(GL_TEXTURE_3D, map_tex);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, config.get_bool("filter", true) ? GL_LINEAR : GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	auto texels = generateTexels(config.get_bool("city", true));
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, MAP_SIZE, MAP_SIZE, MAP_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, texels);

	GLuint quad_vbo;
	glGenVertexArrays(1, &quad_vao);
	glGenBuffers(1, &quad_vbo);
	glBindVertexArray(quad_vao);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*) 0);
	glBindVertexArray(0);

	cam = new Camera(glm::vec3(0.2, 0.2, 0.2));
}

extern bool keys[];

void camUpdate() {
	int x = 0, y = 0;
	if (keys[GLFW_KEY_A]) x = -1;
	else if (keys[GLFW_KEY_D]) x = 1;
	if (keys[GLFW_KEY_S]) y = -1;
	else if (keys[GLFW_KEY_W]) y = 1;

	if (keys[GLFW_KEY_LEFT_SHIFT]) cam->speed = config.get_float("cam_shift_speed", 0.01f);
	else cam->speed = config.get_float("cam_speed", 0.1f);

	cam->ProcessMove(x, y, 1.0f, gravity);

	if (gravity) {
		glm::vec3 pos = cam->pos;
		int depth = 11;
		for (int i = 0; i < 10; i++) {
			if (solid_at(pos.x, pos.y - i / 256.0f, pos.z)) {
				depth = i;
				break;
			}
		}
		if (depth < 8) {
			cam->pos += glm::vec3(0, (5 - depth) / 256.0f, 0);
		} else {
			cam->pos += glm::vec3(0, -1 / 256.0f, 0);
		}
	}
}

void gameUpdate() {
	using namespace std;

	camUpdate();

	if (solid_at(cam->pos.x, cam->pos.y, cam->pos.z))
		cout << "collision at " << cam->pos.x << endl;

	shaders->Bind();
	//cout << "val = " << val << endl;
	glBindTexture(GL_TEXTURE_3D, map_tex);
	glUniform1f(shaders->GetLocation("val"), val);
	glUniform1i(shaders->GetLocation("width"), width);
	glUniform1i(shaders->GetLocation("height"), height);
	glUniform3fv(shaders->GetLocation("cam_pos"), 1, glm::value_ptr(cam->pos));
	glUniform3fv(shaders->GetLocation("cam_dir"), 1, glm::value_ptr(cam->front));
	glUniform3fv(shaders->GetLocation("cam_right"), 1, glm::value_ptr(cam->right));
	glUniform3fv(shaders->GetLocation("cam_up"), 1, glm::value_ptr(cam->up));
	glBindVertexArray(quad_vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void gameEnd() {

}

void gameResize(int w, int h) {
	glViewport(0, 0, w, h);
	width = w;
	height = h;
}

void changeVal(double amount) {
	using namespace std;
	val += amount * config.get_float("val_step", 0.02f);
	float val_min = config.get_float("val_min", 0.02f);
	if (val <= val_min) val = val_min;
	cout << "val = " << val << endl;
}

bool firstMouse = true;
float lastX, lastY;

void gameMouse(float x, float y) {
	if (firstMouse) {
		lastX = x;
		lastY = y;
		firstMouse = false;
	}

	float xo = x - lastX;
	float yo = lastY - y;
	lastX = x;
	lastY = y;

	cam->ProcessLook(xo, yo);
}