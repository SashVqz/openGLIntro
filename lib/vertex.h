#pragma once

#include "Vector4f.h"

typedef struct {
	libMath::vector4f vertexPos;
	libMath::vector4f vertexNormal;
	libMath::vector4f vertexColor;
	libMath::vector4f vertexUv;
} vertex_t;

typedef struct {
	float Ka, Kd, Ks;
	int shinny;
} material_t;