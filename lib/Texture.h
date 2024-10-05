#pragma once

#include "Common.h"

typedef struct {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
} pixel_t;

class Texture
{
public:
	std::vector<pixel_t> colores;
	int w = 0;
	int h = 0;
	std::string fileName = "";
	unsigned int GLId = -1;

	Texture() {};
	Texture(int w, int h, pixel_t color);
	Texture(std::string fileName);
	void bind(int textureUnit);
};

