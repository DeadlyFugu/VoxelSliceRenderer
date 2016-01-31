#pragma once
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtc\type_ptr.hpp"

class Camera
{
public:
	Camera(
		glm::vec3 pos = glm::vec3(0, 0, 0),
		glm::vec3 up = glm::vec3(0, 1, 0),
		float yaw = -90, float pitch = 0);
	~Camera();
	glm::vec3 pos;
	glm::vec3 up;
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 worldup;
	float yaw;
	float pitch;
	float speed;
	float sensitivity;
	float zoom;
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix(float aspectRatio);
	void ProcessMove(int x, int y, float delta, bool fps_move);
	void ProcessLook(int x, int y);
private:
	void UpdateCameraVectors();
};

