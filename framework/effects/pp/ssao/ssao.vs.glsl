/*
	uniforms: {

		"tDiffuse":     { type: "t", value: null },
		"tDepth":       { type: "t", value: null },
		"size":         { type: "v2", value: new THREE.Vector2( 512, 512 ) },
		"cameraNear":   { type: "f", value: 1 },
		"cameraFar":    { type: "f", value: 100 },
		"fogNear":      { type: "f", value: 5 },
		"fogFar":       { type: "f", value: 100 },
		"fogEnabled":   { type: "i", value: 0 },
		"onlyAO":       { type: "i", value: 0 },
		"aoClamp":      { type: "f", value: 0.3 },
		"lumInfluence": { type: "f", value: 0.9 }

	},
*/
varying vec2 vUv;

void main() 
{
	vUv = gl_MultiTexCoord0.xy;
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
}