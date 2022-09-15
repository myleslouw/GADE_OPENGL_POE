#include "CamManager.h"

#include <iostream>
int CamIndex = 0;
Camera globalCamera = Camera();


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