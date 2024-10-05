#pragma once

#include "Common.h"
#include "Vector4f.h"
#include "Mat4x4f.h"
#include "InputManager.h"

typedef enum {
    point = 0,
    directional = 1
} lightType;

class Light
{
public:
    libMath::vector4f pos = { 0, 0, 0, 0 };
    libMath::vector4f rot = { 0, 0, 0, 0 };
    libMath::vector4f scale = { 1, 1, 1, 0 };

    float Ia = 0.2f, Id = 1.0f, Is = 1;
    int enable = 1;

    libMath::vector4f color = { 1, 1, 1, 1 };
    lightType type = point;
    libMath::vector4f direction = { 0, -1, 0, 0 };

    Light(libMath::vector4f pos = { 0, 0, 0, 0 }, lightType type = point, libMath::vector4f direction = { 0, -1, 0, 0 }, libMath::vector4f color = { 1, 1, 1, 0 })
        : pos(pos), type(type), direction(direction), color(color) {}

    void setDirection(libMath::vector4f direction) {
        this->direction = direction;
    }

    void setScale(libMath::vector4f scale) {
        this->scale = scale;
    }

    void setColor(libMath::vector4f color) {
        this->color = color;
    }

    void setPos(libMath::vector4f pos) {
        this->pos = pos;
    }

    virtual void move(float deltaTime) {

    }
};

class OrbitalLight : public Light {
public:
    libMath::vector4f center = { 0,0,0,0 };
    float speed = 1.0f;
    float radious = 1.0f;

    OrbitalLight(libMath::vector4f center = { 0,0,0,0 }, float radious = 1.0f, float speed = 1.0f, libMath::vector4f color = { 1,1,1,1 }) : center(center), radious(radious), speed(speed)
    {
        this->color = color;
        type = point;
        pos.z -= radious;
        pos = pos + center;
    }

    virtual void move(float deltaTime) override {
        float angle = (speed * deltaTime);
        this->rot.y += angle;
        auto rmat = libMath::make_rotate(0, rot.y, 0);

        pos = { 0, 0, -radious, 1 };
        pos = rmat * pos;
        pos = pos + center;
        pos.w = 1;
    }
};