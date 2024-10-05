#pragma once

#include "Light.h"

class orbitalLight : public Light
{
public:
    float radius = 1;
    float speed = 1;
    float angle = 0;
    libMath::vector4f center = { 0, 0, 0, 0 }; 

    void move(float deltaTime) override {
        angle += speed * deltaTime;
        libMath::mat4x4f translationMatrix = libMath::make_translate(center.x, center.y, center.z);
        libMath::mat4x4f rotationMatrix = libMath::rotY(libMath::degreesToRadians(angle);
        libMath::mat4x4f scalingMatrix = libMath::make_scale(radius, 1, radius);

        libMath::matrix4f transformationMatrix = translationMatrix * rotationMatrix * scalingMatrix;

        libMath::vector4f position = transformationMatrix * libMath::vector4f(0, 0, 0, 1);
    }

    void setRadius(float r) {
        radius = r;
    }

    void setSpeed(float s) {
        speed = s;
    }

    void setAngle(float a) {
        angle = a;
    }

    void setCenter(const libMath::vector4f &c) {
        center = c;
    }
};