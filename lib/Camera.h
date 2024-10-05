#pragma once

#include "Vector4f.h"
#include "Mat4x4f.h"
#include "InputManager.h"
#include "collider.h"

class Camera
{
public:
	libMath::vector4f position;
	libMath::vector4f orientation;
	libMath::vector4f lookAt;

	SphereCollider* coll = nullptr;

	float fovy = 0.0f;
	float aspectRatio = 0.0f;
	float zNear = 0;
	float zFar = 0;

	Camera(libMath::vector4f position, libMath::vector4f up, libMath::vector4f lookAt, float fovy, float aspectRatio, float zNear, float zFar)
	{
		this->orientation = normalize(up);
		this->position = position;
		this->lookAt = lookAt;

		this->fovy = fovy;
		this->aspectRatio = aspectRatio;
		this->zNear = zNear;
		this->zFar = zFar;

		coll = new SphereCollider(0.0000001f, position);
	}

	libMath::mat4x4f computeViewMatrix() 
	{
		libMath::mat4x4f m = libMath::make_identityf();
		libMath::vector4f up, forward, right;

		forward = libMath::normalize(lookAt - position);
		forward.w = 0;

		right = libMath::normalize(libMath::cross(forward, (orientation)));
		right.w = 0;

		up = libMath::cross(right, forward);
		up.w = 0;

		m.data2D[0][0] = right.x;
		m.data2D[1][0] = right.y;
		m.data2D[2][0] = right.z;

		m.data2D[0][1] = up.x;
		m.data2D[1][1] = up.y;
		m.data2D[2][1] = up.z;

		m.data2D[0][2] = -forward.x;
		m.data2D[1][2] = -forward.y;
		m.data2D[2][2] = -forward.z;

		m.data2D[3][0] = -(libMath::dot(right, position));
		m.data2D[3][1] = -(libMath::dot(up, position));
		m.data2D[3][2] = libMath::dot(forward, position);

		return libMath::transpose(m);
	}

	libMath::mat4x4f computeProjectionMatrix() 
	{
		libMath::mat4x4f projMat;

		projMat.rows[0] = {1.0f / (aspectRatio * tan(fovy * 0.5f)), 0, 0, 0};
		projMat.rows[1] = {0, 1.0f / tan(fovy * 0.5f), 0, 0};
		projMat.rows[2] = {0, 0, -(zFar + zNear) / (zFar - zNear), (2.0f * zFar * zNear) / (zFar - zNear)};
		projMat.rows[3] = {0 ,0, -1, 1};

		return (projMat);
	}

	virtual void move(float deltaTime)
	{
		float speed = 0.5f;

		if (InputManager::keysState[GLFW_KEY_LEFT]) {
			position.x -= deltaTime * speed;
			//lookAt.x -= deltaTime * speed;
		}

		if (InputManager::keysState[GLFW_KEY_RIGHT]) {
			position.x += deltaTime * speed;
			//lookAt.x += deltaTime * speed;
		}

		if (InputManager::keysState[GLFW_KEY_UP]) {
			position.z += deltaTime * speed;
			//lookAt.z += deltaTime * speed;
		}

		if (InputManager::keysState[GLFW_KEY_DOWN]) {
			position.z -= deltaTime * speed;
			//lookAt.z -= deltaTime * speed;
		}
	}
};