#include "global.h"

state *global::_state = 0;

state::state()
{
	matrixProjection = ZMatrix();
	matrixProjectionInverse = ZMatrix();
	matrixModelView = ZMatrix();
	matrixModelViewInverse = ZMatrix();
	matrixNormal = ZMatrix();
	sceneNum = 0;
}

state* global::GetState()
{
	if (!_state)
		_state = new state();
	return _state;

}