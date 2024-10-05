#pragma once

#include "Vector4f.h"

namespace libMath {

	typedef struct {
		
		union {
			struct {
				vector4f r1;
				vector4f r2;
				vector4f r3;
				vector4f r4;
			};

			vector4f rows[4];
			float data1D[16];
			float data2D[4][4];
		};

	} mat4x4f;

	inline mat4x4f make_identityf() {
		mat4x4f res = { .rows = {
			{1, 0, 0, 0},
			{0, 1, 0, 0},
			{0, 0, 1, 0},
			{0, 0, 0, 1}}
		};

		return res;
	}

	inline vector4f operator * (mat4x4f m, vector4f v) {
		vector4f res = { 0, 0, 0, 0 };

		for (int i = 0; i < 4; i++) {
			res.data[i] = libMath::dot(m.rows[i], v);
		}

		return res;
	}

	inline mat4x4f operator * (mat4x4f m1, mat4x4f m2) {
		mat4x4f res = make_identityf();

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				res.rows[i].data[j] = 0;
				for (int k = 0; k < 4; k++) {
					res.rows[i].data[j] += m1.rows[i].data[k] * m2.rows[k].data[j];
				}
			}
		}

		return res;
	}

	inline mat4x4f operator * (mat4x4f m, float s) {
		mat4x4f res = m;

		for (int i = 0; i < 4; i++) {
			res.rows[i].x *= s;
			res.rows[i].y *= s;
			res.rows[i].z *= s;
		}

		return res;
	}

	inline mat4x4f operator + (mat4x4f m1, mat4x4f m2) {
		mat4x4f res = m1;

		for (int i = 0; i < 4; i++) {
			res.rows[i].x += m2.rows[i].x;
			res.rows[i].y += m2.rows[i].y;
			res.rows[i].z += m2.rows[i].z;
			res.rows[i].w += m2.rows[i].w;
		}

		return res;
	}

	inline mat4x4f operator - (mat4x4f m1, mat4x4f m2) {
		mat4x4f res = m1;

		for (int i = 0; i < 4; i++) {
			res.rows[i].x -= m2.rows[i].x;
			res.rows[i].y -= m2.rows[i].y;
			res.rows[i].z -= m2.rows[i].z;
			res.rows[i].w -= m2.rows[i].w;
		}

		return res;
	}

	inline mat4x4f make_translate(float x, float y, float z) {
		mat4x4f res = make_identityf();
		
		res.rows[0].w = x;
		res.rows[1].w = y;
		res.rows[2].w = z;

		return res;
	}

	inline mat4x4f rotX(float angle) {
		mat4x4f res = { .rows = {
			{1, 0, 0, 0},
			{0, cos(angle), -sin(angle), 0},
			{0, sin(angle), cos(angle), 0},
			{0, 0, 0, 1}}
		};

		return res;
	}

	inline mat4x4f rotY(float angle) {
		mat4x4f res = { .rows = {
			{cos(angle), 0, sin(angle), 0},
			{0, 1, 0, 0},
			{-sin(angle), 0, cos(angle), 0},
			{0, 0, 0, 1}}
		};

		return res;
	}

	inline mat4x4f rotZ(float angle) {
		mat4x4f res = { .rows = {
			{cos(angle), -sin(angle), 0, 0},
			{sin(angle), cos(angle), 0, 0},
			{0, 0, 1, 0},
			{0, 0, 0, 1}}
		};

		return res;
	}

	inline std::string toString(mat4x4f m) {
		std::string res = "";

		for (int i = 0; i < 4; i++) {
			res += libMath::toString(m.rows[i]) + "\n";
		}

		return res;
	}

	inline mat4x4f make_rotate(float angleX, float angleY, float angleZ) {
		mat4x4f res = rotX(angleX);

		res = res * rotY(angleY);
		res = res * rotZ(angleZ);

		return res;
	}

	inline mat4x4f make_scale(float x, float y, float z) {
		mat4x4f res = make_identityf();

		res.rows[0].x = x;
		res.rows[1].y = y;
		res.rows[2].z = z;

		return res;
	}

	inline mat4x4f transpose(mat4x4f m) {
		mat4x4f res = make_identityf();

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				res.rows[i].data[j] = m.rows[j].data[i];
			}
		}

		return res;
	}

	inline mat4x4f inverse(mat4x4f m) {
		mat4x4f res = make_identityf();

		float det = m.rows[0].x * (m.rows[1].y * m.rows[2].z - m.rows[1].z * m.rows[2].y) - m.rows[0].y * (m.rows[1].x * m.rows[2].z - m.rows[1].z * m.rows[2].x) + m.rows[0].z * (m.rows[1].x * m.rows[2].y - m.rows[1].y * m.rows[2].x);

		if (det == 0) {
			return res;
		}

		float invdet = 1 / det;

		res.rows[0].x = (m.rows[1].y * m.rows[2].z - m.rows[1].z * m.rows[2].y) * invdet;
		res.rows[0].y = (m.rows[0].z * m.rows[2].y - m.rows[0].y * m.rows[2].z) * invdet;
		res.rows[0].z = (m.rows[0].y * m.rows[1].z - m.rows[0].z * m.rows[1].y) * invdet;
		res.rows[0].w = 0;

		res.rows[1].x = (m.rows[1].z * m.rows[2].x - m.rows[1].x * m.rows[2].z) * invdet;
		res.rows[1].y = (m.rows[0].x * m.rows[2].z - m.rows[0].z * m.rows[2].x) * invdet;
		res.rows[1].z = (m.rows[0].z * m.rows[1].x - m.rows[0].x * m.rows[1].z) * invdet;
		res.rows[1].w = 0;

		res.rows[2].x = (m.rows[1].x * m.rows[2].y - m.rows[1].y * m.rows[2].x) * invdet;
		res.rows[2].y = (m.rows[0].y * m.rows[2].x - m.rows[0].x * m.rows[2].y) * invdet;
		res.rows[2].z = (m.rows[0].x * m.rows[1].y - m.rows[0].y * m.rows[1].x) * invdet;
		res.rows[2].w = 0;

		res.rows[3].x = -(m.rows[3].x * res.rows[0].x + m.rows[3].y * res.rows[1].x + m.rows[3].z * res.rows[2].x);
		res.rows[3].y = -(m.rows[3].x * res.rows[0].y + m.rows[3].y * res.rows[1].y + m.rows[3].z * res.rows[2].y);
		res.rows[3].z = -(m.rows[3].x * res.rows[0].z + m.rows[3].y * res.rows[1].z + m.rows[3].z * res.rows[2].z);
		res.rows[3].w = 1;

		return res;
	}

};