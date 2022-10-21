#pragma once
//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

#include "AnimationManager.h"

class ChessAnimation
{
	//this class controls the animation of the chess pieces

public:

	ChessAnimation();

	//array of the piecs positions
	glm::vec3 chessPieces[16] = {
		{ glm::vec3(-7, 1, 7) },
		{ glm::vec3(-7, 1, 5) },
		{ glm::vec3(-5, 1, 7) },
		{ glm::vec3(-5, 1, 5) },
		{ glm::vec3(-3, 1, 7) },
		{ glm::vec3(-3, 1, 5) },
		{ glm::vec3(-1, 1, 7) },
		{ glm::vec3(-1, 1, 5) },
		{ glm::vec3(1, 1, 7) },
		{ glm::vec3(1, 1, 5) },
		{ glm::vec3(3, 1, 7) },
		{ glm::vec3(3, 1, 5) },
		{ glm::vec3(5, 1, 5) },
		{ glm::vec3(5, 1, 7) },
		{ glm::vec3(7, 1, 7) },
		{ glm::vec3(7, 1, 5) },
	};


	glm::vec3 testPiece;

	void MovePiece(GLfloat deltaTime);

	void AnimatePiece(int index, GLfloat deltaTime);


	~ChessAnimation();

private:

	int speedDirection;
	int maxY, minY;
	bool pieceCompleted = false;


};

