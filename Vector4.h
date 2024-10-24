#ifndef VECTOR4_H
#define VECTOR4_H

#include<cmath>
#include<stdexcept>

#include "Vector3.h"

template <typename T>
struct Vector4 {
	union {
		T data[4];
		struct {
			T x, y, z, w;
		};
		struct {
			T r, g, b, a;
		};
		struct {
			T A, B, C, D;
		};
	};

	Vector4() {
		data[0] = 0;
		data[1] = 0;
		data[2] = 0;
		data[3] = 0;
	};

	Vector4(T x, T y, T z, T w) {
		data[0] = x;
		data[1] = y;
		data[2] = z;
		data[3] = w;
	};

	Vector4(const Vector4<T>& v) {
		data[0] = v.data[0];
		data[1] = v.data[1];
		data[2] = v.data[2];
		data[3] = v.data[3];
	};

	// overload operators
	Vector4<T>& operator=(const Vector4<T>& v) {
		data[0] = v.data[0];
		data[1] = v.data[1];
		data[2] = v.data[2];
		data[3] = v.data[3];
		return *this;
	}

	T& operator[](int index) {
		if (index < 0 || index >= 4) throw std::out_of_range("Index out of range");
		return data[index];
	}

	const T& operator[](int index) const {
		if (index < 0 || index >= 4) throw std::out_of_range("Index out of range");
		return data[index];
	}

	Vector4<T> operator+(const Vector4<T>& v) {
		return Vector4<T>(data[0] + v.data[0], data[1] + v.data[1], data[2] + v.data[2], data[3] + v.data[3]);
	}

	Vector4<T> operator-(const Vector4<T>& v) {
		return Vector4<T>(data[0] - v.data[0], data[1] - v.data[1], data[2] - v.data[2], data[3] - v.data[3]);
	}

	Vector4<T> operator*(const T& k) {
		return Vector4<T>(data[0] * k, data[1] * k, data[2] * k, data[3] * k);
	}

	Vector4<T> operator/(const T& k) {
		if (k == 0) throw std::invalid_argument("Division by zero");
		return Vector4<T>(data[0] / k, data[1] / k, data[2] / k, data[3] / k);
	}

	Vector4<T> operator-() {
		return Vector4<T>(-data[0], -data[1], -data[2], -data[3]);
	}

	T dot(const Vector4<T>& v) {
		return data[0] * v.data[0] + data[1] * v.data[1] + data[2] * v.data[2] + data[3] * v.data[3];
	}

	// cross is different for 4D vectors

	T magnitude() {
		return sqrt(data[0] * data[0] + data[1] * data[1] + data[2] * data[2] + data[3] * data[3]);
	}

	Vector4<T> normalize() {
		T magnitude = magnitude();
		if (magnitude == 0) throw std::invalid_argument("Normalizing zero vector");
		return Vector4<T>(data[0] / magnitude, data[1] / magnitude, data[2] / magnitude, data[3] / magnitude);
	}

	// convert to 3D vector
	Vector3<T> toVector3() {
		return Vector3<T>(data[0], data[1], data[2]);
	}

	Vector3<T> toVector3Homogeneous() {
		return Vector3<T>(data[0] / data[3], data[1] / data[3], data[2] / data[3]);
	}
};

// define common types
typedef Vector4<float> Vector4f;
typedef Vector4<int> Vector4i;
typedef Vector4<short> Vector4s;

#endif // VECTOR4_H