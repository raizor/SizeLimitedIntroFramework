#include "global.h"

state *global::_state = 0;

state::state()
{
	matrixProjection = ZMatrix();
	matrixProjectionInverse = ZMatrix();
	matrixModelView = ZMatrix();
	matrixModelViewInverse = ZMatrix();
	matrixNormal = ZMatrix();
}

state* global::GetState()
{
	if (!_state)
		_state = new state();
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
