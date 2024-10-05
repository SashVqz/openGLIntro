#pragma once

#include "Common.h"
#include "Camera.h"
#include "Vector4f.h"
#include "InputManager.h"
#include "Render.h"

class CameraFPS : public Camera
{
public:
	float radious = 1.0f;
	float speed = 0.1f;
	
	libMath::vector4f direction = { 0,0,0,0 };
	libMath::vector4f lookAtOrig = { 0,0,-1,1 };
	libMath::vector4f rot = { 0,0,0,0 };
	libMath::vector4f strafeDirection = libMath::normalize({1, 0, 0, 0});

	double oldX = InputManager::mouseState.posX, oldY = InputManager::mouseState.posY;

	bool isMovingUp, isMovingDown, isMovingRight, isMovingLeft = false;
	bool canMoveUp, canMoveDown, canMoveRight, canMoveLeft = true;

	CameraFPS(float speed, libMath::vector4f position, libMath::vector4f up, libMath::vector4f lookAt, float fovy, float aspectRatio, float zNear, float zFar) : Camera(position, up, lookAt, fovy, aspectRatio, zNear, zFar), speed(speed)
	{
		lookAtOrig = lookAt - position;
		direction = normalize(lookAtOrig);
		radious = length(lookAtOrig);
	}

	virtual void move(float deltaTime) override {
		if (Render::r->cameraCollision(SPHERE)) {
			canMoveUp = (isMovingUp) ? false : canMoveUp;
			canMoveDown = (isMovingDown) ? false : canMoveDown;
			canMoveLeft = (isMovingLeft) ? false : canMoveLeft;
			canMoveRight = (isMovingRight) ? false : canMoveRight;
		} else {
			canMoveUp = true;
			canMoveDown = true;
			canMoveLeft = true;
			canMoveRight = true;
		}

		float speedX = oldY - InputManager::mouseState.posY;
		float speedY = oldX - InputManager::mouseState.posX;

		if (InputManager::keysState[GLFW_KEY_UP] && canMoveUp) {
			position = position + direction * speed * deltaTime;
			lookAt = lookAt + direction * speed * deltaTime;
			isMovingUp = true;
		} else {
			isMovingUp = false;
		}

		if (InputManager::keysState[GLFW_KEY_DOWN] && canMoveDown) {
			position = position - direction * speed * deltaTime;
			lookAt = lookAt - direction * speed * deltaTime;
			isMovingDown = true;
		} else {
			isMovingDown = false;
		}

		if (InputManager::keysState[GLFW_KEY_LEFT] && canMoveDown) {
			position = position - strafeDirection * speed * deltaTime;
			lookAt = lookAt - strafeDirection * speed * deltaTime;
			isMovingLeft = true;
		} else {
			isMovingLeft = false;
		}

		if (InputManager::keysState[GLFW_KEY_RIGHT] && canMoveRight) {
			position = position + strafeDirection * speed * deltaTime;
			lookAt = lookAt + strafeDirection * speed * deltaTime;
			isMovingRight = true;
		} else {
			isMovingRight = false;
		}

		rot.y += speedY * 0.5f;
		rot.x += speedX * 0.5f;

		auto mRot = libMath::make_rotate(libMath::degreesToRadians(rot.x), libMath::degreesToRadians(rot.y), 0.0f);
		libMath::vector4f p = {0, 0, -radious, 1};
		p = mRot * p;
		p = p + position;

		lookAt = p;

		direction = normalize(lookAt - position);
		strafeDirection = normalize(cross(direction, orientation));

		oldX = InputManager::mouseState.posX;
		oldY = InputManager::mouseState.posY;

		this->coll->updateCamera(position);
	}
};