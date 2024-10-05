#pragma once
#define _USE_MATH_DEFINES

#include "Common.h"

namespace libMath {

	typedef struct {

		union {
			struct {
				float x, y, z, w;
			};
			struct {
				float r, g, b, a;
			};
			float data[4];
		};

	} vector4f;

	inline vector4f make_vector4f(float x, float y, float z, float w) {
		vector4f v = { x, y, z, w };

		return v;
	}

	inline bool operator == (vector4f v1, vector4f v2) {
		for (int i = 0; i < 4; i++) {
			if (v1.data[i] != v2.data[i]) {
				return false;
			}
		}

		return true;
	}

	inline bool operator != (vector4f v1, vector4f v2) {
		return !(v1 == v2);
	}

	inline float length(vector4f v) {
		return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
	}

	inline vector4f normalize(vector4f v) {
		float n = sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));

		vector4f res = { v.x / n, v.y / n, v.z / n, v.w };
		return res;
	}

	inline vector4f operator + (vector4f v1, vector4f v2) {
		vector4f res = { 0, 0, 0, 0 };

		for (int i = 0; i < 4; i++) {
			res.data[i] += v1.data[i] + v2.data[i];
		}

		return res;
	}

	inline vector4f operator - (vector4f v1, vector4f v2) {
		vector4f res = { 0, 0, 0, 0 };

		for (int i = 0; i < 4; i++) {
			res.data[i] += v1.data[i] - v2.data[i];
		}

		return res;
	}

	inline vector4f operator*(vector4f v, float s) {
		vector4f res = { 0, 0, 0, 0 };

		for (int i = 0; i < 4; i++) {
			res.data[i] = v.data[i] * s;
		}

		return res;
	}

	inline float degreesToRadians(float degrees) {
		return degrees * (M_PI / 180.0f);
	}

	inline float dot(vector4f v1, vector4f v2) {
		float res = 0;

		for (int i = 0; i < 4; i++) {
			res += v1.data[i] * v2.data[i];
		}

		return res;
	}

	inline vector4f cross(vector4f v1, vector4f v2) {
		vector4f res = { 0, 0, 0, 0 };

		res.x = v1.y * v2.z - v1.z * v2.y;
		res.y = v1.z * v2.x - v1.x * v2.z;
		res.z = v1.x * v2.y - v1.y * v2.x;

		return res;
	}

	inline std::string toString(vector4f v) {
		return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ", " + std::to_string(v.w) + ")";
	}
};