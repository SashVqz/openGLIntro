#define GLAD_GL_IMPLEMENTATION
#include "lib/Common.h"

#include "lib/Vector4f.h"
#include "lib/Mat4x4f.h"
#include "lib/Quaternion4f.h"
#include "lib/vertex.h"
#include "lib/Render.h"
#include "lib/InputManager.h"
#include "lib/Object3D.h"
#include "lib/Camera.h"
#include "lib/CameraFPS.h"

int enemiesDestroyed = 0;

class cubo3D : public Object3D {

public:
	cubo3D():Object3D("data/cubeBL.trs", {0, 0, 0, 1}) {
		
	}

	virtual void move(double timeStep) override {
		if (InputManager::keysState[GLFW_KEY_B]) {
			delete this->tex;
			this->tex = new Texture(512, 512, { 0xFF, 0, 0, 0xFF });
		}
		
		if (InputManager::keysState[GLFW_KEY_V]) {
			delete this->tex;
			this->tex = new Texture(512, 512, { 0xFF, 0xFF, 0, 0xFF });
		}
	}
};

class Bullet : public Object3D 
{
public:
	libMath::vector4f direction = { 0, 1, 0, 0 };
	
	Bullet(int bulletType, libMath::vector4f pos) : Object3D("data/bullet.trs", pos) {
		this->createCollider();

		this->rotation.z = (bulletType == BULLET_ENEMY) ? M_PI : 0;
		this->direction = (bulletType == BULLET_ENEMY) ? libMath::vector4f{ 0, -1, 0, 0 } : libMath::vector4f{ 0, 1, 0, 0 };

		this->type = bulletType;
		this->position = pos;
		this->scale = { 0.1f, 0.1f, 0.1f, 0.1f };
		this->tex = new Texture("data/bullet.png");
	}

	virtual void move(double timeStep) override {
		this->position = this->position + (this->direction * timeStep);

		if (Render::r->collisionType(this, BULLET_ALLY) ) {
			Render::r->deleteObject(this);
		}

		if (std::abs(position.y) > 3) Render::r->deleteObject(this);
		this->updateModelMatrix();
	}
};

class Enemy : public Object3D 
{
public:
	bool movingRight = true;

	Enemy(libMath::vector4f pos) : Object3D("data/ship.trs", pos) {
		this->rotation.z = M_PI;
		this->type = ENEMY;
		this->tex = new Texture("data/ship.png");
		this->createCollider();
	}

	virtual void move(double timeStep) override {
		if (movingRight) {
			this->position.x += timeStep * 0.5f;
			if (this->position.x >= 3) {
				movingRight = false;
				this->position.y -= 0.25f;
			}
		}
		else {
			this->position.x -= timeStep * 0.5f;
			if (this->position.x <= -3) {
				movingRight = true;
				this->position.y -= 0.25f;
			}
		}

		if (this->position.y < -2) {
			std::cout << "Game Over!" << std::endl;
			Render::r->gameOver = true;
		}

		if (rand() % 100 < 1) Render::r->putObject(new Bullet(BULLET_ENEMY, this->position));

		this->updateModelMatrix();

		if (Render::r->collisionType(this, BULLET_ALLY)) {
			enemiesDestroyed++;
			std::cout << "Enemies destroyed: " << enemiesDestroyed << std::endl;			
			Render::r->deleteObject(this);

			if (enemiesDestroyed == 10) {
				std::cout << "You Win!" << std::endl;
				Render::r->gameOver = true;
			}
			else {
				Object3D* enemy = new Enemy({ -3, 2, 0, 1 });
				enemy->createCollider();
				Render::r->putObject(enemy);
			}
		}
	}
};

class SpaceShip : public Object3D 
{
public:
	bool oneShot = false;
	float time = 0;
	float cooldown = 2.0f;

	SpaceShip(libMath::vector4f pos) :Object3D("data/ship.trs", pos) {
		this->type = SPACESHIP;
		this->tex = new Texture("data/ship.png");
		this->createCollider();
	}

	virtual void move(double timeStep) override {
		if (time > cooldown) {
			oneShot = false;
			time = 0;
		}
		else time += timeStep;

		if (InputManager::keysState[GLFW_KEY_SPACE] && !oneShot) {
			Render::r->putObject(new Bullet(BULLET_ALLY, this->position));
			oneShot = true;
		}

		if (InputManager::keysState[GLFW_KEY_W]) this->position.y += timeStep * 0.5f;
		if (InputManager::keysState[GLFW_KEY_S] && this->position.y >= -2) this->position.y -= timeStep * 1.2f;
		if (InputManager::keysState[GLFW_KEY_A] && this->position.x >= -3) this->position.x -= timeStep * 1.2f;
		if (InputManager::keysState[GLFW_KEY_D] && this->position.x <= 3) this->position.x += timeStep * 1.2f;

		this->updateModelMatrix();
		
		if (Render::r->collisionType(this, BULLET_ENEMY) || Render::r->collisionType(this, ENEMY)) {
			std::cout << "Game Over!" << std::endl;
			Render::r->gameOver = true;
		}
	}
};


void nivel0(Render* r)
{
	r->initGL("Practica 6 Demo", 1080, 720);

	std::vector<Object3D*> objectList;

	// Camera FPS
	Camera* cam = new CameraFPS(5.0f, { 0, 0, 3, 1 }, { 0, 1, 0, 0 }, { 0, 0, 0, 1 }, M_PI / 4.0f, 4.0f / 3.0f, 0.01f, 100.0f);
	r->putCamera(cam);

	Object3D* g = new Object3D("./data/ground.trs", { 0, 0, 0, 1 });
	g->scale = { 100.0f, 1.0f, 100.0f, 0.0f };
	objectList.push_back(g);

	// Texture Sphere 3D 
	Object3D* s = new Object3D("./data/sphere.trs", { 0, 2, 0, 1 });
	s->createSphereCollider();
	s->type = SPHERE;
	objectList.push_back(s);

	Object3D* mercury = new Object3D("./data/sphere.trs", { 6, 2, 0, 1 });
	mercury->createSphereCollider();
	mercury->type = SPHERE;
	objectList.push_back(mercury);

	// Sun
	Object3D* sun = new Object3D("./data/sphere.trs", { 12, 2, 0, 1 });
	sun->createSphereCollider();
	sun->type = SPHERE;
	objectList.push_back(sun);

	Light* l1 = new Light({ 0, 0, 3, 1 }, point);
	l1->setPos({ -10, 0, 0, 1 });
	r->putLight(l1);

	Light* l2 = new Light({ 0, 0, 3, 1 }, point);
	l2->setPos({ 10, 0, 0, 1 });
	l2->setColor({ 0, 1, 0, 1 });
	r->putLight(l2);

	OrbitalLight* l3 = new OrbitalLight({0, 0, 3, 1}, 10.0f, 1.0f);
	l3->setPos({ 0, 10, 0, 1 });
	l3->setColor({ 0, 0, 1, 1 });
	r->putLight(l3);

	for (auto& obj : objectList) {
		r->putObject(obj);
	}
}

void nivel1(Render* r) 
{
	r->initGL("Space Invaders", 720, 980);

	std::vector<Object3D*> objectList;

	Object3D* ship = new SpaceShip({ 0, -1, 0, 1 });
	ship->createCollider();
	objectList.push_back(ship);

	Object3D* enemy = new Enemy({ -3, 2, 0, 1 });
	enemy->createCollider();
	objectList.push_back(enemy);

	for (auto& obj : objectList) {
		r->putObject(obj);
	}

	Camera* cam = new Camera({ 0, 0, 5, 1 }, { 0, 1, 0, 0 }, { 0, 0, 0, 1 }, M_PI / 4.0f, 4.0f / 3.0f, 0.01f, 100.0f);
	r->putCamera(cam);

	Light* l1 = new Light({ 0, 0, 3, 1 }, point);
	l1->setPos({ 0, 0, 3, 1 });
	l1->setColor({ 1, 1, 1, 1 });
	r->putLight(l1);
}

class Planet : public Object3D {
public:
	libMath::vector4f center = { 0, 0, 0, 0 };
	float speed = 1.0f;
	float radious = 6.0f;

	Planet(std::string fileName, libMath::vector4f pos, float speed = 1.0f, float radious = 6.0f) : Object3D(fileName, pos), speed(speed), radious(radious) {

	}

	virtual void move(double timeStep) override {
		float angle = (speed * timeStep);
		this->rotation.y += angle;
		auto rmat = libMath::make_rotate(0, rotation.y, 0);

		position = { 0, 0, -radious, 1 };
		position = rmat * position;
		position = position + center;
		position.w = 1;

		this->updateModelMatrix();
	}
};

void solarSystemLevel(Render* r)
{
	r->initGL("Solar System", 1080, 720);

	std::vector<Object3D*> objectList;

	// Camera FPS
	Camera* cam = new CameraFPS(5.0f, { 0, 0, 10, 1 }, { 0, 1, 0, 0 }, { 0, 0, 0, 1 }, M_PI / 4.0f, 4.0f / 3.0f, 0.01f, 100.0f);
	r->putCamera(cam);

	// Sun
	Object3D* sun = new Object3D("./data/sphere.trs", { 0, 0, 0, 1 });
	sun->scale = {2.0f, 2.0f, 2.0f, 1};
	sun->mat.Ka = 0.8f;
	sun->mat.Kd = 1.0f;
	sun->mat.Ks = 0.5f;
	sun->mat.shinny = 32;
	sun->createSphereCollider();
	sun->type = SPHERE;
	objectList.push_back(sun);
	
	// Mercury
	Object3D* mercury = new Planet("./data/sphere.trs", { 10, 0, 0, 1 }, 0.3f, 10);
	mercury->createSphereCollider();
	mercury->type = SPHERE;
	objectList.push_back(mercury);

	// Lights (only sun emmits light)
	Light* sunLight1 = new Light({ 0, 0, 0, 1 }, point);
	sunLight1->setScale({10, 10, 10, 0});
	sunLight1->setPos({ 0, 0, 0, 1 });
	sunLight1->setColor({ 1, 1, 0, 1 });
	r->putLight(sunLight1);

	for (auto& obj : objectList) {
		r->putObject(obj);
	}
}

int main(int argc, char** argv)
{
	Render *render = new Render();

	nivel0(render);
	//nivel1(render);
	//solarSystemLevel(render);

	render->mainLoop();

	return 0;
}