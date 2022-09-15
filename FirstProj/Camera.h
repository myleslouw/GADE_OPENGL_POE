#pragma once
#include <GL\glew.h>
#include <stdio.h>

#include <glm/glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <GLFW\glfw3.h>

#include <vector>
#include "CameraPosition.h"

#include "Mesh.h"

class Camera
{
public:

	Camera();

	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);

	void keyControl(bool* keys, GLfloat deltaTime);
	void mouseControl(GLfloat xChange, GLfloat yChange);

	glm::mat4 calculateViewMatrix();

	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	void ChangeCameraPosition();

	~Camera();

private:

	GLfloat yaw;
	GLfloat pitch;

	GLfloat moveSpeed;
	GLfloat turnSpeed;

	//an array of camera positions
	CameraPosition camPositions[3] = {
		{ CameraPosition(glm::vec3(0, 10, 10), -90, -45) },		//45 deg
		{ CameraPosition(glm::vec3(0, 20, 0), -90, -90) },		//top down
		{ CameraPosition(glm::vec3(20, 2, 0), -180, 0) }		//side
	};


	void update();

};