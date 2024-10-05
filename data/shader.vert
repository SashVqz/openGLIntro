#version 330

uniform mat4 MVP;
uniform mat4 M;

attribute vec4 vPos;
attribute vec4 vColor;
attribute vec4 vNormal;
attribute vec4 vUv;

out vec4 fColor;
out vec3 fPos;
out vec3 fNormal;
out vec2 fUv;

void main() {
	fPos = (M * vPos).xyz;
	fNormal = (inverse(transpose(M)) * vNormal).xyz;
	fNormal = normalize(fNormal);
	
	gl_Position = MVP * vPos;

	fColor = vColor;
	fUv = vUv.xy;
}