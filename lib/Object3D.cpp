#include "Object3D.h"
#include "collider.h"

Object3D::Object3D() {
	id = idCounter++;
}

void printData(Object3D obj) {
	std::cout << "Object3D ID: " << obj.id << std::endl;
	
	for (int i = 0; i < obj.vertexList.size(); i++) {
		std::cout << "Vertex " << i << ": " << obj.vertexList[i].vertexPos.x << " " << obj.vertexList[i].vertexPos.y << " " << obj.vertexList[i].vertexPos.z << std::endl;
	}

	for (int i = 0; i < obj.vertexList.size(); i++) {
		std::cout << "Color " << i << ": " << obj.vertexList[i].vertexColor.r << " " << obj.vertexList[i].vertexColor.g << " " << obj.vertexList[i].vertexColor.b << std::endl;
	}

	for (int i = 0; i < obj.idList.size(); i++) {
		std::cout << "ID " << i << ": " << obj.idList[i] << std::endl;
	}
}

Object3D::Object3D(std::string fileName, libMath::vector4f pos) {
	position = pos;
	rotation = { 0, 0, 0, 1 };
	scale = { 1, 1, 1, 1 };
	modelMatrix = libMath::make_identityf();

	this->loadFromFile(fileName.c_str());

	updateModelMatrix();
}

void Object3D::createCollider() {
	coll = new Collider();

	for (int i = 0; i < idList.size(); i += 3) {
		coll->addTriangle(vertexList[idList[i]], vertexList[idList[i + 1]], vertexList[idList[i + 2]]);
	}
}

void Object3D::createSphereCollider() {
	sphrColl = new SphereCollider(libMath::length((vertexList.at(0).vertexPos - position)), position);
}

void Object3D::loadFromFile(const char* fileName) {
	//std::cout << "Leyendo desde fichero\n";

	id = idCounter++;
	std::string line;

	std::ifstream f(fileName, std::ios_base::in);

	if (!f.is_open()) {
		std::cout << "ERROR Fichero " << fileName << " no encontrado\n";
		return;
	}

	bool existsNormal = false;
	while (std::getline(f, line, '\n')) {
		std::istringstream str(line);
		std::string key;

		str >> key;

		if (key[0] != '#') {
			if (key == "vert") {
				libMath::vector4f pos;
				str >> pos.x >> pos.y >> pos.z;
				pos.w = 1.0f;
				vertex_t vert;
				vert.vertexPos = pos;
				vertexList.push_back(vert);
			} else if (key == "color") {
				vertex_t &v = vertexList.back();
				//str >> v.vertexColor.r >> v.vertexColor.g >> v.vertexColor.b >> v.vertexColor.a;
				v.vertexColor = { 1, 1, 1, 1 };
			} else if (key == "face") {
				for (int i = 0; i < 3; i++) {
					int f = 0;
					str >> f;
					idList.push_back(f);
				}
			} else if (key == "svert") {
				std::string fileName;
				str >> fileName;
				prg->addShader(fileName);
			} else if (key == "sfrag") {
				std::string fileName;
				str >> fileName;
				prg->addShader(fileName);
			} else if (key == "normal") {
				vertex_t& v = vertexList.back();
				str >> v.vertexNormal.x >> v.vertexNormal.y >> v.vertexNormal.z >> v.vertexNormal.w;
				existsNormal = true;
			} else if (key == "uv") {
				vertex_t& v = vertexList.back();
				str >> v.vertexUv.x >> v.vertexUv.y;
			}
		}
	}

	prg->link();
	this->tex = nullptr;

	//printData(*this);
}

void Object3D::createTriangle() {
	position = { 0, 0, 0, 1 };
	rotation = { 0, 1, 0, 1 };
	scale = { 0, 0, 0, 1 };

	vertexList = {
		{
			{0, 0.5f, 0, 1}, // Position
			{0, 0, 1, 0}, // Normal
			{1, 0, 0, 1} // Color
		},
		{
			{-0.5f, -0.5f, 0, 1}, // Position
			{0, 0, 1, 0}, // Normal
			{0, 1, 0, 1} // Color
		},
		{
			{0.5f, -0.5f, 0, 1}, // Position
			{0, 0, 1, 0}, // Normal
			{0, 0, 1, 1} // Color
		}
	};

	idList = { 1, 0, 2 };
	modelMatrix = libMath::make_identityf();
}

void Object3D::createCube() {
	position = { 0, 0, 0, 1 };
	rotation = { 0, 1, 0, 1 };
	scale = { 0, 0, 0, 1 };

	vertexList = {
		{
		    {-0.5f, 0.5f, 0.5f, 1}, // Position
			{0, 0, 1, 0}, // Normal
			{1, 0, 0, 1} // Color
		},
		{
			{-0.5f, -0.5f, 0.5f, 1}, // Position
			{0, 0, 1, 0}, // Normal
			{0, 1, 0, 1} // Color
		},
		{
			{0.5f, -0.5f, 0.5f, 1}, // Position
			{0, 0, 1, 0}, // Normal
			{0, 0, 1, 1} // Color
		},
		{
			{0.5f, 0.5f, 0.5f, 1}, // Position
			{0, 0, 1, 0}, // Normal
			{1, 0, 0, 1} // Color
		},
		{
			{-0.5f, 0.5f, -0.5f, 1}, // Position
			{0, 0, -1, 0}, // Normal
			{0, 1, 0, 1} // Color
		},
		{
		    {-0.5f, -0.5f, -0.5f, 1}, // Position
			{0, 0, -1, 0}, // Normal
			{0, 0, 1, 1} // Color
		},
		{
			{0.5f, -0.5f, -0.5f, 1}, // Position
			{0, 0, -1, 0}, // Normal
			{1, 0, 0, 1} // Color
		},
		{
			{0.5f, 0.5f, -0.5f, 1}, // Position
			{0, 0,-1, 0}, // Normal
			{0, 1, 0, 1} // Color
		}
	};

	idList = { 0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7, 0, 4, 7, 0, 7, 3, 1, 5, 6, 1, 6, 2, 0, 1, 5, 0, 5, 4, 3, 7, 6, 3, 6, 2 };
    modelMatrix = libMath::make_identityf();
}

void Object3D::move(double timeStep) {
	if (InputManager::keysState[GLFW_KEY_A]) position.x -= 0.2f * timeStep;
	if (InputManager::keysState[GLFW_KEY_D]) position.x += 0.2f * timeStep;
	if (InputManager::keysState[GLFW_KEY_W]) position.y += 0.2f * timeStep;
	if (InputManager::keysState[GLFW_KEY_S]) position.y -= 0.2f * timeStep;

	if (InputManager::keysState[GLFW_KEY_Q]) rotation.y += 0.8f * timeStep;
	if (InputManager::keysState[GLFW_KEY_E]) rotation.y -= 0.8f * timeStep;

	if (InputManager::keysState[GLFW_KEY_Z]) scale.x -= 0.1f * timeStep;
	if (InputManager::keysState[GLFW_KEY_Z]) scale.y -= 0.1f * timeStep;
    if (InputManager::keysState[GLFW_KEY_Z]) scale.z -= 0.1f * timeStep;
	if (InputManager::keysState[GLFW_KEY_X]) scale.x += 0.1f * timeStep;
	if (InputManager::keysState[GLFW_KEY_X]) scale.y += 0.1f * timeStep;
	if (InputManager::keysState[GLFW_KEY_X]) scale.z += 0.1f * timeStep;

	updateModelMatrix();
}

void Object3D::updateModelMatrix() {
	libMath::mat4x4f translationMatrix = libMath::make_translate(position.x, position.y, position.z);
	libMath::vector4f quaternion = libMath::make_quaternion(rotation.x, rotation.y, rotation.z, rotation.w);
	libMath::mat4x4f rotationMatrix = libMath::make_rotate(rotation.x, rotation.y, rotation.z);
	libMath::mat4x4f scaleMatrix = libMath::make_scale(scale.x, scale.y, scale.z);

	modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
}