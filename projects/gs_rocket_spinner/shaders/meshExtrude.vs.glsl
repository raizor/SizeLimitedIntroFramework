attribute float time;
attribute vec3 direction;
attribute vec3 customColor;
attribute float seed;
attribute vec3 posNormal;

uniform float globalTime;

varying vec3 vColor;
varying vec3 vNormal;
varying float extra;

const vec3 axisx = vec3(1.0, 0.0, 0.0);
const vec3 axisy = vec3(0.0, 1.0, 0.0);
const vec3 axisz = vec3(0.0, 0.0, 1.0);

const vec3 center = vec3(0.0, 0.0, 0.0);

const float strength = 150.0;

void main() {

	vColor = customColor;
	vNormal = normal;


	// time
	float localTime = time + globalTime;
	float modTime = mod( localTime, 1.0 );
	float accTime = modTime * modTime;

	vec3 animated = vec3( position.x, position.y , position.z  );

	vec3 directionVec = normalize(position);

	float xangle = dot(axisx, directionVec) * 2.4;
	float yangle = dot(axisy, directionVec) * 3.2;
	float zangle = dot(axisz, directionVec) * 2.5;

	//float cosx = cos(globalTime + xangle);
	float sinx = sin(globalTime + xangle);
	float cosy = cos(globalTime + yangle);
	//float siny = sin(globalTime + yangle);
	float cosz = cos(globalTime + zangle);
	//float sinz = sin(globalTime + zangle);

	vec3 animatedDirection = direction;

	float mm = directionVec.z * sinx * cosy * cosz * strength;

	if (mm < -10.0) {
		mm = -10.0;
	}

	animated += posNormal * mm;

	animated += animatedDirection*(17.0-mm*0.1);

	extra = max(0.0, mm*0.01 );

	vec4 mvPosition = modelViewMatrix * vec4( animated, 1.0 );

	gl_Position = projectionMatrix * mvPosition;

}