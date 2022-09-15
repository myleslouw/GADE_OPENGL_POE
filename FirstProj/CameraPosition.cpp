#include "CameraPosition.h"

CameraPosition::CameraPosition()
{

}
CameraPosition::CameraPosition(glm::vec3 _position, float _yaw, float _pitch)
{
	position = _position;
	pitch = _pitch;
	yaw = _yaw;
}

CameraPosition::~CameraPosition()
{

}


