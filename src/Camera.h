

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraMovements {
	FORWARD,
	BACKWARD,
	RIGHT,
	LEFT,
	UP,
	DOWN
};

class Camera
{
public:

	float pitch;
	float yaw;

	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;


	float movSpeed = 0.2f;
	float mouseSpeed = 0.2f;
	float zoom;

	Camera() {
		position = glm::vec3(2.0f, 2.0f, 2.0f);
		//up = glm::vec3(0.0f, 0.0f, 1.0f);
		worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		yaw = -90.0f;
		pitch = 0.0f;
		zoom = 45.0f;

		//updateCameraVectors();

	}

	// direction is not tied to the world transform.
	// x - forward / backward
	// y - up / down
	// z - left / right
	void processKeys(glm::vec3 direction, float deltatime);
	void processMouse(glm::vec2 offset);

	void updateCameraVectors();


	// VIEW MATRIX
	glm::mat4 getViewMatrix() {
		return glm::lookAt(position, position + front, up);
	}

	glm::mat4 getProjectionMatrix(float aspectRatio, float nearPlane = 0.1f, float farPlane = 100.0f) const {
		return glm::perspective(glm::radians(zoom), aspectRatio, nearPlane, farPlane);
	}

	

};

