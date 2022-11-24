#include "FPSCounter.h"
FPSCounter::FPSCounter()
{
	currentTime = 0.0;
	prevTime = 0.0;
	timeDifference = 0.0;
	counter = 0;
}

void FPSCounter::ShowFPS(GLFWwindow* window, double time)
{
	currentTime = time;
	timeDifference = currentTime - prevTime;
	counter++;

	if (timeDifference >= 1.0/ 30.0)
	{
		std::string FPS = std::to_string(round((1.0 / timeDifference) * counter));
		
		std::string titleWithFPS = "FPS: " + FPS;
		glfwSetWindowTitle(window, titleWithFPS.c_str());
		prevTime = currentTime;
		counter = 0;
	}
}