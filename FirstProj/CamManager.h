#pragma once
#include "Camera.h"

//GLOBAL INSTANCES SO I CAN CALL CAMERA FUNCTIONS FROM WINDOW::HANDLEKEYS INSTEAD OF CAMERA::KEYCONTROL

extern int CamIndex;
extern Camera globalCamera;
extern bool CameraLocked;

void IncrementIndex();
void DecrementIndex();

void LockCamera();
void UnlockCamera();
void AlternateLock();

