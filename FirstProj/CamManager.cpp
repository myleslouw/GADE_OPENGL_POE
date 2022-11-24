#include "CamManager.h"

int CamIndex = 0;
Camera globalCamera = Camera();

//be able to increment and decrement index
void IncrementIndex()
{
	printf("CAMERA INCREMENT");
	CamIndex++;
	if (CamIndex > 2)
	{
		CamIndex = 0;
	}
}

void DecrementIndex()
{
	printf("CAMERA DECREMENT");
	CamIndex--;
	if (CamIndex < 0)
	{
		CamIndex = 2;
	}
}


