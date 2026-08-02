#include <Camera.h>

void Camera::processKeys(glm::vec3 direction, float deltatime) {
	float velocity = movSpeed * deltatime;
	position += (front * direction.x) * velocity;
	position += (right * direction.z) * velocity;
	position += (up * direction.y) * velocity;
}


void Camera::processMouse(glm::vec2 offset) {
	glm::vec2 deltaOffset = offset * mouseSpeed;

	yaw += offset.x;
	pitch += offset.y;

	pitch = glm::clamp(pitch, -89.0f, 89.0f);

	updateCameraVectors();

}

void Camera::updateCameraVectors() {
	glm::vec3 newFront;
	newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	newFront.y = sin(glm::radians(pitch));
	newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(newFront);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}