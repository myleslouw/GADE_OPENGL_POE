#pragma once
#include "Camera.h"

//GLOBAL INSTANCES SO I CAN CALL CAMERA FUNCTIONS FROM WINDOW::HANDLEKEYS INSTEAD OF CAMERA::KEYCONTROL

extern int CamIndex;
extern Camera globalCamera;

void IncrementIndex();
void DecrementIndex();

