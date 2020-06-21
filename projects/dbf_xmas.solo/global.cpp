#include "global.h"

IntroState *global::_state = 0;

IntroState::IntroState()
{
	matrixProjection = ZMatrix();
	matrixProjectionInverse = ZMatrix();
	matrixModelView = ZMatrix();
	matrixModelViewInverse = ZMatrix();
	matrixNormal = ZMatrix();
}

IntroState* global::GetState()
{
	if (!_state)
		_state = new IntroState();
	return _state;
}

void global::InitializeFramework(float viewportWidth, float viewportHeight, int randSeed)
{
	/*
	RES_X = viewportWidth;
	RES_Y = viewportHeight;
	*/
#ifdef ENABLE_RANDOM
	_state.randomSeed = new int();
	*_state.randomSeed = randSeed;
#endif
}
