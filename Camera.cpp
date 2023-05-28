#include"Camera.h"

Camera::Camera(int width, int height,float Rotation, glm::vec3 position){
	
	Position = position;
	rotation = Rotation;
	Camera::width = width;
	Camera::height = height;
}

void Camera::matrix(float FieldofView, float near, float far, Shader& shader, const char* uniform)
{
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	view = glm::lookAt(Position, Position + orientation, up);

	projection = glm::perspective(glm::radians(FieldofView), (float)width / height, near, far);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(rotation), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(projection * view * model));
}



void Camera::inputs(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		rotation += 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS){
		Position += speed * -glm::normalize(glm::cross(orientation, up));
	}
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS){
		Position += speed * -orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS){
		Position += speed * glm::normalize(glm::cross(orientation, up));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
		Position += speed * up;
	}
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS){
		Position += speed * -up;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE){
		speed = 0.001f;
	}
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
		rotation -= 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		Position += speed * orientation;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		if (firstclick){
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstclick = false;
		}
		double mouseX;
		double mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		float rotationY = sensitivity * (float)(mouseX - (width / 2)) / width;
		float rotationX = sensitivity * (float)(mouseY - (height / 2)) / height;
		glm::vec3 neworientation = glm::rotate(orientation, glm::radians(-rotationX), glm::normalize(glm::cross(orientation, up)));
		if (abs(glm::angle(neworientation, up) - glm::radians(90.0f)) <= glm::radians(85.0f)) {
			orientation = neworientation;
		}
		orientation = glm::rotate(orientation, glm::radians(rotationY), up);
		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE){
		firstclick = true;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

