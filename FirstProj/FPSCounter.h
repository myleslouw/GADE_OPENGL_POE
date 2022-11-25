#pragma once
#include <string>
#include <GLFW/glfw3.h>

class FPSCounter
{
public:
	FPSCounter();
	void ShowFPS(GLFWwindow* window, double time);

private:
	double prevTime, currentTime, timeDifference;

	unsigned int counter;
};

