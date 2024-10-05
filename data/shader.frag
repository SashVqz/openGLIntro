#version 330

struct light_t {
	vec4 pos;
	vec4 color;
	vec4 direction;
	float Ia;
	float Id;
	float Is;
	int type;
	int enable;
};

struct material_t {
	float Ka, Kd, Ks;
	int shinny;
	int textureEnable;
};

uniform light_t light0;
uniform light_t light1;
uniform light_t light2;
uniform light_t light3;
uniform light_t light4;
uniform light_t light5;

uniform vec4 eyePos;
uniform material_t material;
uniform sampler2D textureColor;

in vec4 fColor;
in vec3 fPos;
in vec3 fNormal;
in vec2 fUv;

vec4 calculateColor(light_t light) {
	vec4 finalColor = fColor;

   	if (material.textureEnable == 1) {
		finalColor = fColor * texture(textureColor, fUv);
	}
	
	float ambiental = 1;
	float diffuse = 1;
	float specular = 1;

	if (light.enable == 1) {
		ambiental = light.Ia * material.Ka;
		
		vec3 L = vec3(0, 0, 0);
		vec3 N = fNormal;

		if (light.type == 0) L = light.pos.xyz - fPos;
		if (light.type == 1) L = light.direction.xyz;

		L = normalize(L);
		N = normalize(N);
		
		diffuse = light.Id * material.Kd * max(dot(L, N), 0);

		vec3 R = normalize(reflect(L, N));
		vec3 V = normalize(fPos - eyePos.xyz);

		specular = light.Is * material.Ks * pow(max(dot(R, V), 0), material.shinny);
	}

	return finalColor*((ambiental + diffuse + specular) * light.color); 
}

void main() {
	gl_FragColor = calculateColor(light0) + calculateColor(light1) + calculateColor(light2) + calculateColor(light3) + calculateColor(light4) + calculateColor(light5);
	//gl_FragColor = calculateColor(light1);
	//gl_FragColor = vec4(light1.color);
}