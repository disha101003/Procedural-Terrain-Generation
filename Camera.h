
#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>


#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

#include"shader.h"

class Camera
{
public:
	glm::vec3 Position;
	glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	bool firstclick = true;
	float speed = 0.01f;
	float sensitivity = 100.0f;
	float rotation = 0.0f;

	int width;
	int height;


	Camera(int width, int height,float Rotation, glm::vec3 position);
	void matrix(float FieldofView, float near, float far, Shader& shader, const char* uniform);
	void inputs(GLFWwindow* window);
};
#endif