#include "Camera.h"



Camera::Camera(
	glm::vec3 pos,
	glm::vec3 up,
	float yaw, float pitch)
	: front(glm::vec3(0,0,-1)), speed(0.01f), sensitivity(0.25f), zoom(45)
{
	this->pos = pos;
	this->worldup = up;
	this->yaw = yaw;
	this->pitch = pitch;
	this->UpdateCameraVectors();
}


Camera::~Camera()
{
}


glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(this->pos, this->pos + this->front, this->up);
}

glm::mat4 Camera::GetProjectionMatrix(float aspectRatio)
{
	return glm::perspective(glm::radians((double) zoom), (double)aspectRatio, .1, 100.0);
}


void Camera::UpdateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	front.y = sin(glm::radians(this->pitch));
	front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	this->front = glm::normalize(front);
	this->right = glm::normalize(glm::cross(this->front, this->worldup));
	this->up = glm::normalize(glm::cross(this->right, this->front));
}

void Camera::ProcessMove(int x, int y, float delta, bool fps_move) {
	glm::vec2 vel = glm::vec2(x, y) * this->speed * delta;
	glm::vec3 motion = this->front*vel.y + this->right*vel.x;
	if (fps_move) {
		motion.y = 0;
	}
	this->pos += motion;
}

void Camera::ProcessLook(int x, int y) {
	this->yaw += x*this->sensitivity;
	this->pitch += y*this->sensitivity;

	if (this->pitch > 89) this->pitch = 89;
	if (this->pitch < -89) this->pitch = -89;

	this->UpdateCameraVectors();
}