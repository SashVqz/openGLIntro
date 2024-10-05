#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" 

Texture::Texture(int w, int h, pixel_t color) {
	this->w = w;
	this->h = h;
	this->colores.resize(w * h);

	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			this->colores[y * w + x] = color;
		}
	}

	glGenTextures(1, &GLId);
	glBindTexture(GL_TEXTURE_2D, GLId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, colores.data());
}

Texture::Texture(std::string fileName) {
	int numComps = 0;
	pixel_t* datos = (pixel_t*) stbi_load(fileName.c_str(), &this->w, &this->h, &numComps, 4);
	
	this->colores.resize(w * h);

	memcpy(this->colores.data(), datos, sizeof(pixel_t) * w * h);

	stbi_image_free(datos);

	glGenTextures(1, &GLId);
	glBindTexture(GL_TEXTURE_2D, GLId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, colores.data());
}

void Texture::bind(int textureUnit) {
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, GLId);
}