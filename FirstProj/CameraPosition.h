#pragma once
#include <glm/glm.hpp>

//instances of these classes are created to store data about a camera position 
//the camera changes its data to match an objects data to go to another camera angle
class CameraPosition
{
public:
	CameraPosition();

	CameraPosition(glm::vec3 _position, float _yaw, float _pitch);

	glm::vec3 position;

	float pitch, yaw;

	~CameraPosition();
};

