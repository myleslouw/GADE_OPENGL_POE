#include "ChessAnimation.h"

#include <iostream>

ChessAnimation::ChessAnimation()
{
	speedDirection = 1;

	maxY = 2;
	minY = 1;

	testPiece.x = 1;
	testPiece.y = 1;
	testPiece.z = 1;


}


void ChessAnimation::MovePiece(GLfloat deltaTime)
{

	if (animationActive)
	{
		//starts animation
		AnimatePiece(pieceIndex, deltaTime);
	}
	else {
		pieceCompleted = false;
		chessPieces[pieceIndex].y = 1;
		pieceIndex = 0;

	}
}

void ChessAnimation::AnimatePiece(int index, GLfloat deltaTime)
{
	//animates the piece with the index param

	//if the piece is at the bottom and it hasnt reached the top it will do so
	if ((chessPieces[index].y <= minY) && !pieceCompleted)
	{
		speedDirection = 3;
	}
	if (chessPieces[index].y >= maxY)
	{
		//piece has reached top and is coming down
		speedDirection = -3;
		pieceCompleted = true;
	}
	if ((chessPieces[index].y <= minY) && pieceCompleted)
	{
		//stop moving piece and set to starting position
		chessPieces[index].y = 1;
		pieceCompleted = false;

		//increments the index so the next piece will animate
		if (pieceIndex < (sizeof(chessPieces) / sizeof(chessPieces[0])) - 1)
		{
			pieceIndex++;
		}
		else {
			std::cout << "ANIMATION ENDED!";
			animationActive = false;
		}
	}

	//animates the piece
	chessPieces[index].y = chessPieces[index].y + speedDirection * deltaTime;

}

ChessAnimation::~ChessAnimation()
{

}