#include "CamManager.h"

#include <iostream>
int CamIndex = 0;
Camera globalCamera = Camera();
bool CameraLocked = false;


//be able to increment and decrement index
void IncrementIndex()
{
	CamIndex++;
	if (CamIndex > 2)
	{
		CamIndex = 0;
	}
}

void DecrementIndex()
{
	CamIndex--;
	if (CamIndex < 0)
	{
		CamIndex = 2;
	}
}

void LockCamera()
{
	CameraLocked = true;
}

void UnlockCamera()
{
	CameraLocked = false;
}

void AlternateLock()
{
	CameraLocked = !CameraLocked;
}
